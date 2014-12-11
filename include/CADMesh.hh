/* ************************************************
 * GEANT4 CAD INTERFACE
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
#include "G4TriangularFacet.hh"
#include "G4Tet.hh"
#include "G4AssemblyVolume.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

// TETGEN //
#include "tetgen.h"

// Open Asset Importer Library //
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class CADMesh {
  public:
    CADMesh(char * file, double units, G4ThreeVector offset, G4bool reverse);
    CADMesh(char * file, char * type, double units, G4ThreeVector offset, G4bool reverse);
    CADMesh(char * file, char * type, G4Material * material, double quality);
    CADMesh(char * file, char * type, G4Material * material, double quality, G4ThreeVector offset);
    CADMesh(char * file, char * type);
    CADMesh(char * file, char * type, G4Material * material);
    ~CADMesh();

  public:

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
    G4ThreeVector GetTetPoint(G4int index_offset);
 
  public:
    // Load tessellated meshes using ASSIMP.
    G4VSolid* TessellatedMesh();
    G4VSolid* TessellatedMesh(G4int index);
    G4VSolid* TessellatedMesh(G4String name);

    // Load tetrahedral meshes using TETGEN.
    G4AssemblyVolume * TetrahedralMesh();
    
    // Getters and setters for various properties. 
    G4TessellatedSolid * GetSolid() {
        return volume_solid;
    };

    // TODO: This method will be removed in a future version. Use CADMesh::GetFileName instead.
    G4String MeshName() {
        return file_name;
    };

    G4String GetFileName() {
        return this->file_name;
    };

    G4String GetFileType() {
        return this->file_type;
    };

    void SetVerbose(G4int verbose) {
        this->verbose = verbose;
    };

    G4int GetVerbose() {
        return this->verbose;
    };

    void SetUnits(G4double units) {
        this->units = units;
    };

    G4double GetUnits() {
        return this->units;
    };

    void SetOffset(G4ThreeVector offset) {
        this->offset = offset;
    };
    
    G4ThreeVector GetOffset() {
        return this-> offset;
    };

    void SetReverse(G4bool reverse) {
        this->reverse = reverse;
    };

    G4bool GetReverse() {
        return this->reverse;
    };

    void SetMaterial(G4Material* material) {
        this->material = material;
    };

    G4Material* GetMaterial() {
        return this->material;
    };

    void SetQuality(G4double quality) {
        this->quality = quality;
    };

    G4double GetQuality() {
        return this->quality;
    };

    G4AssemblyVolume * GetAssembly() {
        return assembly;
    };

  private:
    // For tesselated meshes.
    G4TessellatedSolid * volume_solid;

    const aiScene* scene;
    aiMesh* m;

    // For tetrahedral meshes.
    tetgenio in, out;
    G4AssemblyVolume * assembly;

    // General properties. 
    G4int verbose;
    G4bool has_mesh;
    G4bool has_solid;

    G4String name;
    G4double units;
    G4ThreeVector offset;
    G4bool reverse;

    G4double quality;
    
    G4Material * material;

    G4String file_name;
    G4String file_type;
};

#endif // CADMesh_HH

