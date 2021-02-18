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

/** \file OneAxisFilter.cpp
	A simple Human-Machine-Interface component providing desired cartesian position
*/

#include "OneAxisFilter.hpp"
#include <ocl/ComponentLoader.hpp>

using namespace RTT;

OneAxisFilter::OneAxisFilter(std::string name) :
		RTT::TaskContext(name),
		inputPosition_port("inputPosition"),
		outputPosition_port("outputPosition"),
		axis_prop("axis", 
				  "Specify which axis to filter (should be one of \"x\", \"y\", or \"z\")", 
				  "")	// illegal value
{
	ports()->addPort(&inputPosition_port);
	ports()->addPort(&outputPosition_port);

	properties()->addProperty(&axis_prop);
}

OneAxisFilter::~OneAxisFilter()
{
}

bool OneAxisFilter::configureHook()
{
	bool		rc ;
	
	rc = ("x" == axis_prop.rvalue()) || 
		("y" == axis_prop.rvalue()) ||
		("z" == axis_prop.rvalue());
	if (!rc)
	{
		log(Error) << "Invalid axis specified '" << axis_prop.rvalue() 
				   << "'. Value should be one of \"x\", \"y\", or \"z\"."
				   << endlog();
	}
			
	return rc;
}

void OneAxisFilter::updateHook()
{
	KDL::Frame		position;
	
	inputPosition_port.Get(position);

	if ("x" == axis_prop.rvalue()) 
	{
		position.p.x(0);
	}
	else if ("y" == axis_prop.rvalue())
	{
		position.p.y(0);
	}
	else if	("z" == axis_prop.rvalue())
	{
		position.p.z(0);
	}
	// else user has changed the axis after we started, so do not filter

	outputPosition_port.Set(position);
}
		
//ORO_CREATE_COMPONENT_TYPE()	// in HMI.cpp
ORO_LIST_COMPONENT_TYPE(OneAxisFilter);
