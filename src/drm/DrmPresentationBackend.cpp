//
// Created by Jesse on 10/3/2020.
//

#include <unistd.h>
#include <sys/epoll.h>

#include <mars/drm/DrmPresentationBackend.hpp>
#include <mars/drm/DrmDeviceResources.hpp>

mars::DrmPresentationBackend::DrmPresentationBackend(const std::string& devicePath)
		: device(devicePath)
{
	if (!device.set_capability(DRM_CLIENT_CAP_ATOMIC, true)) {
		throw std::runtime_error("device does not support atomic API");
	}

	// Query device capabilities to see if it supports what we need
	auto capabilities = device.get_capabilities();
	if (!capabilities.dumbBuffers) {
		throw std::runtime_error("device does not support basic buffers");
	}
	if (!capabilities.vdcInVBlankEvent) {
		throw std::runtime_error("does not support vertical synchronization");
	}

	// Create swap chains for each connected device.
	// TODO: Implement support for new monitors being attached
	mars::DrmDeviceResources resources(device);
	for (auto& connector: resources.get_connectors()) {
		if (connector->connection != DRM_MODE_CONNECTED) {
			continue;
		}

		// TODO: Find encoder if this call fails
		auto encoder = connector.get_current_encoder();
		auto mode = connector.preferred_video_mode();
		if (!encoder.is_available()) {
			std::cerr << "DRM device connector has no available encoder" << std::endl;
			continue;
		}

		swapchains.emplace_back(device, std::move(connector), std::move(encoder), mode);
	}

	// Create an epoll structure to poll events later


	// Last but not least, modeset our device.
	mars::DrmAtomicRequest request(device, this);

	for (auto& swapchain: swapchains) {
		swapchain.modeset(request);
	}

	if (request.commit(DRM_MODE_ATOMIC_TEST_ONLY | DRM_MODE_ATOMIC_ALLOW_MODESET) < 0) {
		throw std::runtime_error("test-only atomic commit failed");
	}

	if (request.commit(DRM_MODE_ATOMIC_ALLOW_MODESET | DRM_MODE_PAGE_FLIP_EVENT) < 0) {
		throw std::runtime_error("mode setting failed");
	}
}

void mars::DrmPresentationBackend::poll_events()
{
	drmEventContext ev = {};
	ev.version = DRM_EVENT_CONTEXT_VERSION;
	ev.page_flip_handler2 = page_flip_handler;

	int epollFd = epoll_create1(0);
	if (epollFd == -1) {
		std::cerr << "could not create epoll file descriptor" << std::endl;
		return;
	}

	epoll_event event = {};
	event.events = EPOLLIN;
	event.data.fd = device.get_fd();

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, device.get_fd(), &event)) {
		std::cerr << "failed to add file descriptor to epoll" << std::endl;
		close(epollFd);
		return;
	}

	auto count = epoll_wait(epollFd, &event, 1, 0);
	if (count > 0 && event.data.fd == device.get_fd()) {
		drmHandleEvent(device.get_fd(), &ev);
	}

	close(epollFd);
}

void
mars::DrmPresentationBackend::page_flip_handler(int fd, uint32_t seq, uint32_t s, uint32_t us, uint32_t vdc, void* ptr)
{
	auto* self = static_cast<DrmPresentationBackend*>(ptr);

	mars::DrmAtomicRequest request(self->device, self);

	for (auto& swapchain: self->swapchains) {
		swapchain.swap_buffers();

		// Get the frame buffer and draw to it
		auto& buf = swapchain.get_current_frame_buffer().get_buffer();
		auto* pixels = static_cast<uint8_t*>(buf.map());
		auto frame = swapchain.get_frame();

		for (int y = 0; y < buf.get_height(); y++) {
			for (int x = 0; x < buf.get_width(); x++) {
				auto offset = buf.get_pitch() * y + x * 4;
				pixels[offset + 0] = (x + frame) % 255;
				pixels[offset + 1] = (y + frame) % 255;
				pixels[offset + 2] = (x + y + frame) % 255;
			}
		}

		swapchain.modeset(request);
	}

	if (request.commit(DRM_MODE_PAGE_FLIP_EVENT) < 0) {
		throw std::runtime_error("mode setting failed");
	}


}
