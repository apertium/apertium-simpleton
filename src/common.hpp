/*
* Copyright (C) 2015-2023, Tino Didriksen <mail@tinodidriksen.com>
*
* This file is part of apertium-simpleton
*
* apertium-simpleton is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* apertium-simpleton is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with apertium-simpleton.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef COMMON_HPP_fc74fd919e58451d89f5d0bba814925a
#define COMMON_HPP_fc74fd919e58451d89f5d0bba814925a

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	#include <QtCore5Compat/QTextCodec>
#endif

#if QT_VERSION >= 0x050400
	#define DATALOCATION QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#else
	#define DATALOCATION QStandardPaths::writableLocation(QStandardPaths::DataLocation)
#endif

#ifdef _MSC_VER
	// warning C4512: assignment operator could not be generated
	#pragma warning (disable: 4512)
	// warning C4456: declaration hides previous local declaration
	#pragma warning (disable: 4456)
	// warning C4458: declaration hides class member
	#pragma warning (disable: 4458)
	// warning C4312: 'operation' : conversion from 'type1' to 'type2' of greater size
	#pragma warning (disable: 4312)
#endif

#endif
