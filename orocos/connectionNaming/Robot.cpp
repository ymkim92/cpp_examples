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

/** \file Robot.cpp
	A simple Human-Machine-Interface component providing desired cartesian position
*/

#include "Robot.hpp"
#include <ocl/ComponentLoader.hpp>

using namespace RTT;

Robot::Robot(std::string name) :
		RTT::TaskContext(name),
		cartesianPosition_desi_port("cartesianPosition_desi")
{
	ports()->addPort(&cartesianPosition_desi_port);
}

Robot::~Robot()
{
}

//ORO_CREATE_COMPONENT_TYPE()	// in HMI.cpp
ORO_LIST_COMPONENT_TYPE(Robot);
