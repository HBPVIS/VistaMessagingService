/*============================================================================*/
/*                                 VistaFlowLib                               */
/*               Copyright (c) 1998-2011 RWTH Aachen University               */
/*============================================================================*/
/*                                  License                                   */
/*                                                                            */
/*  This program is free software: you can redistribute it and/or modify      */
/*  it under the terms of the GNU Lesser General Public License as published  */
/*  by the Free Software Foundation, either version 3 of the License, or      */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU Lesser General Public License for more details.                       */
/*                                                                            */
/*  You should have received a copy of the GNU Lesser General Public License  */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*============================================================================*/
/*                                Contributors                                */
/*                                                                            */
/*============================================================================*/
/* $Id$ */

#ifndef _VMSTESTVOCABULARYCONFIG_H
#define _VMSTESTVOCABULARYCONFIG_H

// Windows DLL build
#if defined(WIN32)  
	#ifdef VMSSIMPLEISOVOCABULARY_EXPORTS
		#define SIMPLEISOVOC_API __declspec(dllexport)
	#else
		#define SIMPLEISOVOC_API __declspec(dllimport)
	#endif
#else // no Windows or static build
	#define SIMPLEISOVOC_API
#endif

#endif //_VISTAVISEXTCONFIG_H
