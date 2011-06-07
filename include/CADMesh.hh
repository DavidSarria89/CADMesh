/* ************************************************
 * GEANT4 VCGLIB/CAD INTERFACE
 *
 * File:      CADMesh.hh
 *
 * Author:    Christopher M Poole,
 * Email:     mail@christopherpoole.net
 *
 * Date:      7th March, 2011
 **************************************************/

#ifndef CADMesh_HH
#define CADMesh_HH

// GEANT4 //
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4TessellatedSolid.hh"
#include "G4AssemblyVolume.hh"

// VCGLIB //
#include "vcg/simplex/vertex/base.h"
#include "vcg/simplex/vertex/component.h"
#include "vcg/simplex/face/base.h"
#include "vcg/simplex/face/component.h"

#include "vcg/complex/complex.h"

// TETGEN //
#include "tetgen.h"

class CADVertex;
class CADFace;

class CADUsedTypes: public vcg::UsedTypes< vcg::Use<CADVertex>::AsVertexType, vcg::Use<CADFace>::AsFaceType>{};

class CADVertex : public vcg::Vertex<CADUsedTypes, vcg::vertex::Coord3d, vcg::vertex::Normal3f> {};
class CADFace : public vcg::Face<CADUsedTypes, vcg::face::VertexRef> {};
class CADTriMesh : public vcg::tri::TriMesh< std::vector<CADVertex>, std::vector<CADFace> > {};


class CADMesh
{
// METHODS //
public:
    CADMesh(char * file, char * type, double units, G4ThreeVector offset, G4bool reverse);
    CADMesh(char * file, char * type);
    CADMesh(char * file, char * type, G4Material * material);
    ~CADMesh();

public:
    G4VSolid* TessellatedMesh();
    G4AssemblyVolume * TetrahedralMesh();

    G4TessellatedSolid* GetSolid() { return volume_solid; };

    G4String MeshName(){ return _file_name; };

    int MeshVertexNumber(){ return m.VertexNumber(); };
    float MeshVolume(){ return m.Volume(); };

    void SetVerbose(int v){ verbose = v; };

private:
    G4ThreeVector GetTetPoint(G4int offset);

// VARS //
private:
    CADTriMesh m;
    tetgenio in, out;

    G4TessellatedSolid * volume_solid;
    G4AssemblyVolume * assembly;

    G4int verbose;
    G4bool has_mesh;
    G4bool has_solid;

    G4String _name;
    G4double _units;
    G4ThreeVector _offset;
    G4bool _reverse;
    
    G4Material * _material;

    char * _file_name;
    G4String _file_type;
};

#endif // CADMesh_HH

