/* ************************************************
 * GEANT4 CAD INTERFACE
 *
 * File:      CADMesh.cc
 *
 * Author:    Christopher M Poole,
 * Email:     mail@christopherpoole.net
 *
 * Date:      7th March, 2011
 **************************************************/

// USER //
#include "CADMesh.hh"


CADMesh::CADMesh(char * file_name, double scale,
        G4ThreeVector offset, G4bool reverse)
{
    this->scale = scale;
    this->offset = offset;
    this->reverse = reverse;

    this->file_name = file_name;
    this->file_type = "ASSIMP";
    this->file_type.toUpper();

    this->material = NULL;
    this->quality = 0;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::CADMesh(char * file_name, char * file_type, double scale,
        G4ThreeVector offset, G4bool reverse)
{
    this->scale = scale;
    this->offset = offset;
    this->reverse = reverse;

    this->file_name = file_name;
    this->file_type = file_type;
    this->file_type.toUpper();

    this->material = NULL;
    this->quality = 0;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::CADMesh(char * file_name, char * file_type,
        G4Material * material, double quality)
{
    this->scale = mm;
    this->offset = G4ThreeVector();
    this->reverse = false;

    this->file_name = file_name;
    this->file_type = file_type;
    this->file_type.toUpper();

    this->material = material;
    this->quality = quality;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::CADMesh(char * file_name, char * file_type)
{
    this->scale = mm;
    this->offset = G4ThreeVector();
    this->reverse = false;

    this->file_name = file_name;
    this->file_type = file_type;
    this->file_type.toUpper();

    this->material = NULL;
    this->quality = 0;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::CADMesh(char * file_name, char * file_type, G4Material * material)
{
    this->scale = mm;
    this->offset = G4ThreeVector();
    this->reverse = false;

    this->file_name = file_name;
    this->file_type = file_type;
    this->file_type.toUpper();

    this->material = material;
    this->quality = 0;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::CADMesh(char * file_name, char * file_type,
        G4Material * material, double quality, G4ThreeVector offset)
{
    this->scale = mm;
    this->offset = offset;
    this->reverse = false;

    this->file_name = file_name;
    this->file_type = file_type;
    this->file_type.toUpper();

    this->material = material;
    this->quality = quality;

    this->has_mesh = false;
    this->has_solid = false;
    this->verbose = 0;
}


CADMesh::~CADMesh()
{
}


G4VSolid* CADMesh::TessellatedMesh()
{
    return TessellatedMesh(0);
}


G4VSolid* CADMesh::TessellatedMesh(G4int index)
{
    Assimp::Importer importer;
    scene = importer.ReadFile(file_name,
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);

    m = scene->mMeshes[index];

    volume_solid = new G4TessellatedSolid(file_name);

    G4ThreeVector point_1;
    G4ThreeVector point_2;
    G4ThreeVector point_3;

    for(unsigned int i=0; i < m->mNumFaces; i++)
    {
        const aiFace& face = m->mFaces[i];

        point_1.setX(m->mVertices[face.mIndices[0]].x * scale + offset.x());
        point_1.setY(m->mVertices[face.mIndices[0]].y * scale + offset.y());
        point_1.setZ(m->mVertices[face.mIndices[0]].z * scale + offset.z());

        point_2.setX(m->mVertices[face.mIndices[1]].x * scale + offset.x());
        point_2.setY(m->mVertices[face.mIndices[1]].y * scale + offset.y());
        point_2.setZ(m->mVertices[face.mIndices[1]].z * scale + offset.z());

        point_3.setX(m->mVertices[face.mIndices[2]].x * scale + offset.x());
        point_3.setY(m->mVertices[face.mIndices[2]].y * scale + offset.y());
        point_3.setZ(m->mVertices[face.mIndices[2]].z * scale + offset.z());
        
        G4TriangularFacet * facet;
        if (reverse == false) {
            facet = new G4TriangularFacet(point_1, point_2, point_3, ABSOLUTE);
        } else {
            facet = new G4TriangularFacet(point_2, point_1, point_3, ABSOLUTE);
        }
        volume_solid->AddFacet((G4VFacet*) facet);
    }

    volume_solid->SetSolidClosed(true);

    if (volume_solid->GetNumberOfFacets() == 0) {
        G4cerr << "CADMesh/TessellatedMesh: "
               << "Load a mesh has 0 faces, " << file_name << " may not exist."
               << G4endl;
        return 0;
    }

    return volume_solid;
}


G4VSolid* CADMesh::TessellatedMesh(G4String name)
{
    Assimp::Importer importer;
    scene = importer.ReadFile(file_name,
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);

    for (unsigned int index = 0; index < scene->mNumMeshes; index++) {
        aiMesh* mesh = scene->mMeshes[index];

        if (strcmp(mesh->mName.C_Str(), name.c_str()))
            return TessellatedMesh(index);
    }

    G4cerr << "Mesh " << name << " not found. Cannot be loaded." << G4endl;
}  


G4AssemblyVolume * CADMesh::TetrahedralMesh()
{
    // USAGE: assembly->MakeImprint(world_logical, assembly_transform_3d, 0); //

    char* fn = (char*) file_name.c_str();

    G4bool do_tet = true;
    if (file_type == "STL") {
        in.load_stl(fn);
    } else if (file_type == "PLY") {
        in.load_ply(fn);
    } else if (file_type == "TET") {
        out.load_tetmesh(fn, 0);
        do_tet = false;
    } else if (file_type == "OFF") {
        out.load_off(fn);
        do_tet = false;
    }

    if (do_tet)
    {
        tetgenbehavior* behavior = new tetgenbehavior();
        behavior->nobisect = 1;
        behavior->plc = 1;
        behavior->quality = quality;

        tetrahedralize(behavior, &in, &out);
    }

#ifdef DEBUG
    G4cout << "Tetrahedra available: " << out.numberoftetrahedra << G4endl;
#endif

    G4cout << "UNITS: " << scale << G4endl;

    assembly = new G4AssemblyVolume();
    G4RotationMatrix * element_rotation = new G4RotationMatrix();
    G4ThreeVector element_position = G4ThreeVector();
    G4Transform3D assembly_transform = G4Translate3D();

    for (int i=0; i<out.numberoftetrahedra; i++) {
        int index_offset = i * 4; /* For a tetrahedron, out.numberofcorners == 4 */

        G4ThreeVector p1 = GetTetPoint(index_offset);
        G4ThreeVector p2 = GetTetPoint(index_offset + 1);
        G4ThreeVector p3 = GetTetPoint(index_offset + 2);
        G4ThreeVector p4 = GetTetPoint(index_offset + 3);

        G4String tet_name = file_name + G4String("_tet_") + G4UIcommand::ConvertToString(i);

        G4VSolid * tet_solid = new G4Tet(tet_name + G4String("_solid"), p1, p2, p3, p4, 0);
        G4LogicalVolume * tet_logical = new G4LogicalVolume(tet_solid, material, tet_name + G4String("_logical"), 0, 0, 0);
        assembly->AddPlacedVolume(tet_logical, element_position, element_rotation);

#ifdef DEBUG
        if (i%1000 == 0) G4cout << "Tetrahedrons added: " << i << G4endl;
#endif
    }

#ifdef DEBUG
    G4cout << "Loading of " << out.numberoftetrahedra << " tetrahedra complete with material: " << material->GetName() << G4endl;
#endif

    return assembly;
}

G4ThreeVector CADMesh::GetTetPoint(G4int index_offset)
{
    return G4ThreeVector(out.pointlist[out.tetrahedronlist[index_offset]*3] * scale - offset.x(),
            out.pointlist[out.tetrahedronlist[index_offset]*3+1] * scale - offset.y(),
            out.pointlist[out.tetrahedronlist[index_offset]*3+2] * scale - offset.z());
}

