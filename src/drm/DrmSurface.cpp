//
// Created by Jesse on 9/27/2020.
//

#include <mars/drm/DrmSurface.hpp>
#include <mars/drm/DrmDeviceResources.hpp>
#include <mars/drm/DrmPlaneResources.hpp>

mars::DrmSurface::DrmSurface(DrmDevice& device, DrmConnector& connector, DrmEncoder& encoder, drmModeModeInfo mode)
		: device(device), encoder(encoder), connector(connector), modeInfo(mode),
		  modeBlob(device, &modeInfo),
		  initialVdc(device, encoder.get_default_vdc().get_id())
{
	DrmPlaneResources planeResources(device);
	auto planes = planeResources.get_planes();
	DrmVideoDisplayController vdc = encoder.get_default_vdc();

	for (auto& plane: planes) {
		if (!plane.is_compatible_with_vdc(vdc)) {
			continue;
		}
		auto type = plane.properties().get_value("type");
		switch (type) {
			case DRM_PLANE_TYPE_PRIMARY:
				primaryPlane.emplace(std::move(plane));
				break;

			case DRM_PLANE_TYPE_CURSOR:
				cursorPlane.emplace(std::move(plane));
				break;

			default:
				std::cout << "unknown plane type: " << type << std::endl;
		}
	}

	if (!primaryPlane.has_value()) {
		throw std::runtime_error("no primary plane found");
	}

	for (int i = 0; i < 2; i++) {
		buffers.emplace_back(device, mode);
	}

	// Finally, perform the modeset
	mars::DrmUniqueAtomicRequestPtr req(drmModeAtomicAlloc());
	prepare(req.get());

	if (drmModeAtomicCommit(device.get_fd(), req.get(),
	                        DRM_MODE_ATOMIC_TEST_ONLY | DRM_MODE_ATOMIC_ALLOW_MODESET, nullptr) < 0) {
		throw std::runtime_error("test-only atomic commit failed");
	}

	if (drmModeAtomicCommit(device.get_fd(), req.get(),
	                        DRM_MODE_ATOMIC_ALLOW_MODESET | DRM_MODE_PAGE_FLIP_EVENT, nullptr) < 0) {
		throw std::runtime_error("mode setting failed");
	}
}

mars::DrmSurface::~DrmSurface()
{
	// Restore the default CRTC so that we do not lock the system
	drmModeSetCrtc(device.get_fd(),
	               initialVdc.get_id(),
	               initialVdc->buffer_id,
	               initialVdc->x,
	               initialVdc->y,
	               &connector->connector_id,
	               1,
	               &initialVdc->mode);
}

void mars::DrmSurface::prepare(drmModeAtomicReq* req)
{
	DrmVideoDisplayController vdc = encoder.get_default_vdc();
	connector.set_property(req, "CRTC_ID", vdc.get_id());
	vdc.set_property(req, "MODE_ID", modeBlob.get_id());
	vdc.set_property(req, "ACTIVE", 1);
	// Grab the currently active framebuffer
	auto& fb = get_current_frame_buffer();
	// Set primary plane properties
	primaryPlane->set_property(req, "FB_ID", fb.get_id());
	primaryPlane->set_property(req, "CRTC_ID", vdc.get_id());
	primaryPlane->set_property(req, "SRC_X", 0);
	primaryPlane->set_property(req, "SRC_Y", 0);
	primaryPlane->set_property(req, "SRC_W", fb.get_buffer().get_width() << 16);
	primaryPlane->set_property(req, "SRC_H", fb.get_buffer().get_height() << 16);
	primaryPlane->set_property(req, "CRTC_X", 0);
	primaryPlane->set_property(req, "CRTC_Y", 0);
	primaryPlane->set_property(req, "CRTC_W", fb.get_buffer().get_width());
	primaryPlane->set_property(req, "CRTC_H", fb.get_buffer().get_height());
}

void mars::DrmSurface::swap_buffers()
{
	frame++;
}

mars::DrmFrameBuffer& mars::DrmSurface::get_current_frame_buffer()
{
	return buffers[frame % buffers.size()];
}

