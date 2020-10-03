//
// Created by Jesse on 10/3/2020.
//

#include <stdexcept>

#include <mars/drm/DrmAtomicRequest.hpp>

mars::DrmAtomicRequest::DrmAtomicRequest(mars::DrmDevice& device, void* userData)
		: device(device),
		  req(drmModeAtomicAlloc()),
		  userData(userData)
{
	if (req == nullptr) {
		throw std::runtime_error("could not allocate atomic request");
	}
}

int mars::DrmAtomicRequest::commit(uint32_t flags)
{
	return drmModeAtomicCommit(device.get_fd(), req.get(), flags, userData);
}
