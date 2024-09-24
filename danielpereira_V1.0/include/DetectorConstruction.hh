#ifndef G4_PCM_DETECTOR_CONSTRUCTION_H
#define G4_PCM_DETECTOR_CONSTRUCTION_H 1

#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomTools.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4Sphere.hh"
#include "Randomize.hh"

namespace G4_PCM
{
    class DetectorConstructionMessenger; // Forward declaration

    class DetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    void SetTargetRotation(G4double rotationAngle); 
    G4VPhysicalVolume* Construct() override;

    void SetTargetThickness(G4double thickness);
    void ConstructArm(); 
	void ConstructSingleBone(); 
	void ConstructOsBone();
	void ConstructFilter();
	void ConstructRealBone(); 
	void ConstructNormalBone(); 
	void ConstructBoneWall();
	void ConstructArmWall();  
	
        G4LogicalVolume* GetGammaDetector() const { return fGammaDetector; }

    private:
        G4double fTargetRotation; 
        G4LogicalVolume* fGammaDetector = nullptr;
        G4double fTargetThickness = 60 * mm; // Valor predeterminado

        G4UIcmdWithADoubleAndUnit* fTargetThicknessCmd;
        DetectorConstructionMessenger* fMessenger; // Pointer to the messenger
        
        G4Box *solidWorld, *solidFilter, *solidHuesoTrabecular, *solidHuesoCortical, *solidSkinP, *solidFatP, *solidMuscleP; 
        G4LogicalVolume *logicBone, *logicMuscle, *logicGrasa, *logicSkin, *logicWorld, *logicFilter, *logicTrabecular, *logicCortical, *logicHuesoTrabecular, *logicHuesoCortical, *logicSkinP, *logicFatP, *logicMuscleP; 
        G4VPhysicalVolume *physBone, *physMuscle, *physGrasa, *physSkin, *physWorld, *physFilter, *physTrabecular, *physCortical, *physHuesoTrabecular, *physHuesoCortical, *physSkinP, *physFatP, *physMuscleP; 
        G4Material *bone, *vacuum, *E_PbWO4, *skin, *grasa, *muscle, *OsBone, *H, *C, *N, *O, *Mg, *P, *S, *Ca, *W, *F, *trabecularBone, *RealOsBone; 
        G4Tubs *solidBone, *solidMuscle, *solidGrasa, *solidSkin, *solidTrabecular, *solidCortical; 
        G4bool isArm, isSingleBone, isOsBone, isFiltered, isRealisticBone, isNormalBone, isBoneWall, isArmWall; 
        G4ThreeVector targetPos, filterPos; 
        G4RotationMatrix *targetRotation; 
        G4Sphere *pore;  
        G4VSolid *porousBone; 
        G4double outerBoneRadius, detectorSizeXY, detectorSizeZ, filterThick, innerBoneRadius, r; 
        
        void DefineMaterials();
        
    };
}

#endif
