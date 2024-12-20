#pragma once

namespace crab
{
	struct CBTransform 
	{
		Mat worldMat = Mat::Identity;
	};

	struct CBCamera
	{
		Mat		viewProjMat = Mat::Identity;
		Vec3	cameraPosition = Vec3::Zero;
		float	pad;
	};
}