// Created on: 1990-12-17
// Created by: Remi Lequette
// Copyright (c) 1990-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _TopoDS_TFace_HeaderFile
#define _TopoDS_TFace_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS_TShape.hxx>

class TopoDS_TFace;
DEFINE_STANDARD_HANDLE(TopoDS_TFace, TopoDS_TShape)

//! A  topological part  of a surface   or  of the  2D
//! space.  The  boundary  is  a   set of  wires   and
//! vertices.
class TopoDS_TFace : public TopoDS_TShape
{
public:
  //! Creates an empty TFace.
  TopoDS_TFace()
      : TopoDS_TShape()
  {
  }

  //! returns FACE.
  Standard_EXPORT TopAbs_ShapeEnum ShapeType() const Standard_OVERRIDE;

  //! Returns an empty TFace.
  Standard_EXPORT virtual Handle(TopoDS_TShape) EmptyCopy() const Standard_OVERRIDE;

  DEFINE_STANDARD_RTTIEXT(TopoDS_TFace, TopoDS_TShape)
};

#endif // _TopoDS_TFace_HeaderFile
