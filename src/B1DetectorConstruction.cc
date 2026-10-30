//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 2.0*env_sizeXY;
  G4double world_sizeZ  = 2.0*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
 
  //所有尺寸参数
    //====不同探测器更改此处参数====
  G4double Active_Area = 1700 * mm *mm;
  G4double Junction_Area = 1900 * mm * mm;
  G4double Si_outer =5.0 *mm;
    //====不同探测器更改此处参数====
    //Si芯片为圆柱体，厚度统一为325um
  G4double Si_chip_Thickness = 325.0 * um;//厚度统一
  G4double Active_Radius = std::sqrt(Active_Area / M_PI);
  G4double Junction_Radius = std::sqrt(Junction_Area / M_PI);
  G4double Si_chip_Radius = Junction_Radius + Si_outer;
  
  //AIR
  G4double Air_Thickness = 100.0 * um;
  G4double Air_Radius = Active_Radius;
  //Al极片
  G4double Al_Thickness = 50.0 * um;
  G4double Al_R1 = Active_Radius;
  G4double Al_R2 = Junction_Radius+2*mm;
  //SiO2 钝化层 固定50um
  G4double SiO2_Thickness = 50.0 * um;
  G4double SiO2_R1 = Al_R2 + 1.0 *mm;
  G4double SiO2_R2 = Si_outer + Junction_Radius ;  
  //外层不锈钢，边缘厚度1mm=====
  G4double Fe_outer = Si_outer + 1.0*mm;
  G4double Fe_Radius =   Junction_Radius + Fe_outer;
  G4double Fe_Thickness = 0.5 * mm;
  G4Material* Fe_mat = nist->FindOrBuildMaterial("G4_Fe");
  G4ThreeVector pos_Fe = G4ThreeVector(0, 0, -Fe_Thickness/2);
    //外层不锈钢为圆柱体，厚度为0.5mm，半径为芯片半径+5mm
  G4Tubs* solidFe =    
    new G4Tubs("Fe",                       //its name
       0, Fe_Radius, Fe_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicFe =                         
    new G4LogicalVolume(solidFe,          //its solid
                        Fe_mat,           //its material
                        "Fe");            //its name
  new G4PVPlacement(0,                     //no rotation
                      pos_Fe,       //at (0,0,0)
                      logicFe,            //its logical volume
                      "Fe",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking            
  //Si芯片为圆柱体，厚度统一为325um，半径为芯片半径+5mm
     G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si");
  G4ThreeVector pos_Si = G4ThreeVector(0, 0, -12.5 * um);

  G4Tubs* solidSi =    
    new G4Tubs("Si",                       //its name
       0, Si_chip_Radius, Si_chip_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicSi =                         
    new G4LogicalVolume(solidSi,          //its solid
                        Si_mat,           //its material
                        "Si");            //its name
    new G4PVPlacement(0,                     //no rotation 
                        pos_Si,       //at (0,0,0)
                        logicSi,            //its logical volume
                        "Si",               //its name
                        logicFe,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
   
        
  //AIR,表面填充物
  G4Material* Air_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4ThreeVector pos_Air = G4ThreeVector(0, 0, Fe_Thickness/2 - Air_Thickness/2); 

  G4Tubs* solidAir =    
    new G4Tubs("Air",                       //its name
       0, Air_Radius, Air_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicAir =                         
    new G4LogicalVolume(solidAir,          //its solid
                        Air_mat,           //its material
                        "Air");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_Air,       //at (0,0,0)
                        logicAir,            //its logical volume
                        "Air",               //its name
                        logicFe,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking  

  //清漆面涂层，等效0.6um的Si
  G4double Layer_Thickness = 0.6 * um;
  G4double Layer_Radius = Active_Radius;
  G4Tubs *solidLayer = new G4Tubs("Layer", 0, Layer_Radius, Layer_Thickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *logicLayer = new G4LogicalVolume(solidLayer, Si_mat, "Layer");
  G4ThreeVector pos_Layer = G4ThreeVector(0, 0, - Air_Thickness/2 + Layer_Thickness/2);
  new G4PVPlacement(0, pos_Layer, logicLayer, "Layer", logicAir, false, 0, checkOverlaps);
 
  //上层Al极片
  G4Material* Al_mat = nist->FindOrBuildMaterial("G4_Al");
  G4ThreeVector pos_Al = G4ThreeVector(0, 0,  Fe_Thickness/2 - Air_Thickness/2 - 25.0 * um);
  G4Tubs* solidAl =    
    new G4Tubs("Al",                       //its name
       Al_R1, Al_R2,Al_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicAl =                         
    new G4LogicalVolume(solidAl,          //its solid
                        Al_mat,           //its material
                        "Al");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_Al,       //at (0,0,0)
                        logicAl,            //its logical volume
                        "Al",               //its name
                        logicFe,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    //上层SiO2钝化 
  G4Material* SiO2_mat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4ThreeVector pos_SiO2 = G4ThreeVector(0, 0,  Fe_Thickness/2 - Air_Thickness/2 - 25.0 * um);
  G4Tubs* solidSiO2 =    
    new G4Tubs("SiO2",                       //its name
       SiO2_R1, SiO2_R2,SiO2_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicSiO2 =                         
    new G4LogicalVolume(solidSiO2,          //its solid
                        SiO2_mat,           //its material
                        "SiO2");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_SiO2,       //at (0,0,0)
                        logicSiO2,            //its logical volume
                        "SiO2",               //its name
                        logicFe,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    //内部的p极
  G4double P_Thickness = 1.5 * um;
  G4ThreeVector pos_P = G4ThreeVector(0, 0,  Si_chip_Thickness/2 -P_Thickness/2);
  G4Tubs* solidP =    
    new G4Tubs("P",                       //its name
       0, Junction_Radius,P_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicP =                         
    new G4LogicalVolume(solidP,          //its solid
                        Si_mat,           //its material
                        "P");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_P,       //at (0,0,0)
                        logicP,            //its logical volume
                        "P",               //its name
                        logicSi,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
  //灵敏体积，占据剩余的Si层
  G4double Sensitive_Thickness = 300 *um;
  G4ThreeVector pos_Sensitive = G4ThreeVector(0, 0, 11*um );
  G4Tubs* solidSensitive =    
    new G4Tubs("Sensitive",                       //its name
       0, Active_Radius,Sensitive_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logicSensitive =                         
    new G4LogicalVolume(solidSensitive,          //its solid
                        Si_mat,           //its material
                        "Sensitive");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_Sensitive,       //at (0,0,0)
                        logicSensitive,            //its logical volume
                        "Sensitive",               //its name
                        logicSi,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
  //AIR 占据剩余的不锈钢区域
  G4double left_Air_Thickness = Fe_Thickness - Air_Thickness-Si_chip_Thickness;
  G4double left_Air_Radius = Si_chip_Radius;
  G4ThreeVector pos_left_Air = G4ThreeVector(0, 0, -Fe_Thickness/2 + left_Air_Thickness/2);
  G4Tubs* solid_left_Air =    
    new G4Tubs("left_Air",                       //its name
       0, left_Air_Radius,left_Air_Thickness/2, 0.*deg, 360.*deg);     //its size
  G4LogicalVolume* logic_left_Air =                         
    new G4LogicalVolume(solid_left_Air,          //its solid
                        Air_mat,           //its material
                        "left_Air");            //its name
    new G4PVPlacement(0,                     //no rotation
                        pos_left_Air,       //at (0,0,0)
                        logic_left_Air,            //its logical volume
                        "left_Air",               //its name
                        logicFe,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
  // Set Shape2 as scoring volume
  //
  /*
  G4ThressVector pos = ThreeVectoor(0,0,0)
  G4Tubs * solid_Air = new  G4Tubs("air"
                                    R1,
                                    R2,
                                    Thickness/2,
                                    0 *deg,
                                    360 *deg);
  G4LogicalVolume * logic_Air = new G4logicalVolume(solid_Air,
                                                    Air_mat,
                                                    "Air")
  G4PhysicalVolunme *physical_air = new G4PVPlacement(0,
                                                    pos,
                                                    logic_Air,
                                                    "Air",
                                                    logicFe,
                                                    false,
                                                    0,
                                                    checkOverlaps);
  */
  fScoringVolume = logicSensitive;
  
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
