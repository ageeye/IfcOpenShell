/********************************************************************************
 *                                                                              *
 * This file is part of IfcOpenShell.                                           *
 *                                                                              *
 * IfcOpenShell is free software: you can redistribute it and/or modify         *
 * it under the terms of the Lesser GNU General Public License as published by  *
 * the Free Software Foundation, either version 3.0 of the License, or          *
 * (at your option) any later version.                                          *
 *                                                                              *
 * IfcOpenShell is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * Lesser GNU General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the Lesser GNU General Public License     *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                              *
 ********************************************************************************/

#ifndef USDSERIALIZER_H
#define USDSERIALIZER_H

#ifdef WITH_USD

#include "../serializers/serializers_api.h"
#include "../ifcgeom_schema_agnostic/GeometrySerializer.h"
#include "../ifcparse/utils.h"

// undefine opencascade Handle macro, because it conflicts with USD
#undef Handle

#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/base/vt/array.h"
#include "pxr/usd/usdGeom/mesh.h"
#include "pxr/usd/usdShade/material.h"

#include <vector>
#include <string>
#include <algorithm>
#include <map>

namespace usd_utils {
  	template<typename T>
  	pxr::VtArray<T> toVtArray(const std::vector<T>& vec) {
    	auto array = pxr::VtArray<T>(vec.size());
		for (std::size_t i = 0; i < vec.size(); ++i)
			array[i] = vec[i];
		return array;
  	}	
}

class SERIALIZERS_API USDSerializer : public WriteOnlyGeometrySerializer {
private:
	std::string filename_;
    bool ready_ = false;
    pxr::UsdStageRefPtr stage_;
	std::size_t unnamed_count_ = 0;
	std::map<std::string, pxr::UsdGeomMesh> meshes_;
	std::map<std::string, pxr::UsdShadeMaterial> materials_;

	inline std::string sanitize(std::string s) {
		std::replace_if(s.begin(), s.end(),
			[](char c) { return c > 122 || c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97); },
			'_');
		return s;
	}

	std::vector<pxr::UsdShadeMaterial> createMaterials(const pxr::UsdGeomMesh&, const std::vector<IfcGeom::Material>&);
    void createLighting();
public:
	USDSerializer(const std::string&, const SerializerSettings&);
	virtual ~USDSerializer();
	bool ready();
	void writeHeader();
	void write(const IfcGeom::TriangulationElement*);
	void write(const IfcGeom::BRepElement*) {}
	void finalize();
	bool isTesselated() const { return true; }
	void setUnitNameAndMagnitude(const std::string&, float) {}
	void setFile(IfcParse::IfcFile*) {}
};

#endif

#endif