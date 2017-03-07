//#******************************************************************************
//#*
//#*      Copyright (C) 2015  Compro Computer Services
//#*      http://openig.compro.net
//#*
//#*      Source available at: https://github.com/CCSI-CSSI/MuseOpenIG
//#*
//#*      This software is released under the LGPL.
//#*
//#*   This software is free software; you can redistribute it and/or modify
//#*   it under the terms of the GNU Lesser General Public License as published
//#*   by the Free Software Foundation; either version 2.1 of the License, or
//#*   (at your option) any later version.
//#*
//#*   This software is distributed in the hope that it will be useful,
//#*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
//#*   the GNU Lesser General Public License for more details.
//#*
//#*   You should have received a copy of the GNU Lesser General Public License
//#*   along with this library; if not, write to the Free Software
//#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//#*
//#*    Please direct any questions or comments to the OpenIG Forums
//#*    Email address: openig@compro.net
//#*
//#*
//#*    Please direct any questions or comments to the OpenIG Forums
//#*    Email address: openig@compro.net
//#*
//#*
//#*****************************************************************************
//#*	authors		Trajce Nikolov Nick openig@compro.net
//#*				Pelle Nordqvist nordqvist@gmail.com
//#*	copyright(c)Compro Computer Services, Inc.
//#*
//#*    Please direct any questions or comments to the OpenIG Forums
//#*    Email address: openig@compro.net
//#*
//#*


#include <Library-Protocol/DeadReckonEntityState.h>

using namespace OpenIG::Library::Protocol;

DeadReckonEntityState::DeadReckonEntityState()
{
}

int DeadReckonEntityState::write(OpenIG::Library::Networking::Buffer &buf) const
{
	buf << (unsigned char)opcode();

	buf << entityID;	
	buf << positionalVelocity.x() << positionalVelocity.y() << positionalVelocity.z();
	buf << orientationalVelocity.z() << orientationalVelocity.y() << orientationalVelocity.z();

	return sizeof(unsigned char) + sizeof(int) + sizeof(float) * 6;
}

int DeadReckonEntityState::read(OpenIG::Library::Networking::Buffer &buf)
{
	unsigned char op;

	buf >> op;
	buf >> entityID;	
	buf >> positionalVelocity.x() >> positionalVelocity.y() >> positionalVelocity.z();
	buf >> orientationalVelocity.z() >> orientationalVelocity.y() >> orientationalVelocity.z();

	return sizeof(unsigned char) + sizeof(int) + sizeof(float) * 6;
}
