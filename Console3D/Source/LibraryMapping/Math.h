#pragma once

#define EIGEN

#ifdef EIGEN

#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace Eigen;

template<class EL, class ER>
inline typename Eigen::MatrixBase<EL>::PlainObject
operator^(
	const Eigen::MatrixBase<EL>& el,
	const Eigen::MatrixBase<ER>& er)
{
	return el.cross3(er);
}

#endif