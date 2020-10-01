//
// Created by Jesse on 9/27/2020.
//

#pragma once

#include <vector>
#include <optional>

#include <mars/drm/DrmConnector.hpp>
#include <mars/drm/DrmDevice.hpp>
#include <mars/drm/DrmDumbBuffer.hpp>
#include <mars/drm/DrmEncoder.hpp>
#include <mars/drm/DrmFrameBuffer.hpp>
#include <mars/drm/DrmPropertyBlob.hpp>
#include <mars/drm/DrmPlane.hpp>

namespace mars
{
	/**
	 * DrmSurface represents a video mode that is active on a display with attached framebuffer. DrmSurface can be used
	 * to blit or copy pixels to, as well as setting the hardware cursor and its position.
	 */
	class DrmSurface
	{
		DrmDevice& device;
		DrmEncoder& encoder;
		DrmConnector& connector;
		drmModeModeInfo modeInfo;
		DrmPropertyBlob modeBlob;
		const DrmVideoDisplayController initialVdc;
		std::vector<DrmFrameBuffer> buffers;
		std::optional<DrmPlane> primaryPlane;
		std::optional<DrmPlane> cursorPlane;

		uint64_t frame{0};

	public:
		DrmSurface(DrmDevice& device, DrmConnector& connector, DrmEncoder& encoder, drmModeModeInfo mode);

		~DrmSurface();

		void prepare(drmModeAtomicReq* req);
		void swap_buffers();
		DrmFrameBuffer& get_current_frame_buffer();
	};
}