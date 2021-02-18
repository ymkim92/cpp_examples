/***************************************************************************
 Copyright (c) 2009 Stephen Roderick <xxxstephen AT theptrgroupxxx DOT comxxx>
                                             (remove the x's above)

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

/** \file Robot.hpp
	A simple Human-Machine-Interface component providing desired cartesian position
*/

#ifndef	__ROBOT_HPP
#define	__ROBOT_HPP 1

#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <kdl/frames.hpp>

/** Simulated robot
 */
class Robot : public RTT::TaskContext
{
protected:
	// DATA INTERFACE

	// *** INPUTS ***

	/// desired cartesian position
	RTT::ReadDataPort<KDL::Frame>			cartesianPosition_desi_port;
	
public:
	Robot(std::string name);
	virtual ~Robot();
};

#endif
