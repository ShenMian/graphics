// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include <cassert>

const Matrix4& Camera::getView() const
{
	return view;
}

const Matrix4& Camera::getProjection() const
{
	return projection;
}
