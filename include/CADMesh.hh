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
#include "G4Material.hh"

// TETGEN //
#ifndef NOTET
#include "tetgen.h"
#endif

// Open Asset Importer Library //
#ifndef NOASSIMP
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#endif

// VCGLIB //
#ifndef NOVCGLIB
#include "vcg/simplex/vertex/base.h"
#include "vcg/simplex/vertex/component.h"
#include "vcg/simplex/face/base.h"
#include "vcg/simplex/face/component.h"
#include "vcg/complex/complex.h"

class CADVertex;
class CADFace;

class CADUsedTypes: public vcg::UsedTypes< vcg::Use<CADVertex>::AsVertexType, vcg::Use<CADFace>::AsFaceType>{};

class CADVertex : public vcg::Vertex<CADUsedTypes, vcg::vertex::Coord3d, vcg::vertex::Normal3f> {};
class CADFace : public vcg::Face<CADUsedTypes, vcg::face::VertexRef> {};
class CADTriMesh : public vcg::tri::TriMesh< std::vector<CADVertex>, std::vector<CADFace> > {};
#endif


class CADMesh {
  public:
    CADMesh(char * file, char * type, double units, G4ThreeVector offset, G4bool reverse);
    CADMesh(char * file, char * type, G4Material * material, double quality);
    CADMesh(char * file, char * type, G4Material * material, double quality, G4ThreeVector offset);
    CADMesh(char * file, char * type);
    CADMesh(char * file, char * type, G4Material * material);
    ~CADMesh();

#ifndef NOTET
  public:
    G4AssemblyVolume * TetrahedralMesh();

    G4AssemblyVolume * GetAssembly() {
        return assembly;
    };

    int get_input_point_count() {
        return in.numberofpoints;
    };

    int get_output_point_count() {
        return out.numberofpoints;
    };

    int get_tetrahedron_count() {
        return out.numberoftetrahedra;
    };

  private:
    tetgenio in, out;
    G4AssemblyVolume * assembly;
#endif

#ifndef NOVCGLIB
  public:
    G4VSolid* TessellatedMesh();

    G4TessellatedSolid * GetSolid() {
        return volume_solid;
    };

    G4String MeshName() {
        return file_name;
    };

    int MeshVertexNumber() {
        return m.VertexNumber();
    };
    
    void SetVerbose(int verbose) {
        this->verbose = verbose;
    };

  private:
    G4ThreeVector GetTetPoint(G4int index_offset);
    CADTriMesh m;
#endif

  private:
    G4TessellatedSolid * volume_solid;
    G4int verbose;
    G4bool has_mesh;
    G4bool has_solid;

    G4String name;
    G4double units;
    G4ThreeVector offset;
    G4bool reverse;

    G4double quality;
    
    G4Material * material;

    char * file_name;
    G4String file_type;
};

#endif // CADMesh_HH

