#include "8.0_SteppingAction.hh"
#include "G4UserLimits.hh"

SteppingAction::SteppingAction(EventAction * eventAction) {fEventAction = eventAction;}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step * step)
{
    G4Track * track = step -> GetTrack();
    G4ThreeVector position = track -> GetPosition();
    worldMaxX =  500.1 * mm; worldMinX = -500.1 * mm; 
    worldMaxY =  500.1 * mm; worldMinY = -500.1 * mm;
    worldMaxZ =  500.1 * mm; worldMinZ = -500.1 * mm;
    if (position.x() < worldMinX || position.x() > worldMaxX || position.y() < worldMinY || position.y() > worldMaxY || position.z() < worldMinZ || position.z() > worldMaxZ)  
    {track -> SetTrackStatus(fStopAndKill); G4cout << " ERROR: Particle outside world bounds!!!" << G4endl;}

    Volume = step -> GetPreStepPoint() -> GetTouchableHandle() -> GetVolume() -> GetLogicalVolume();
    detectorConstruction = static_cast < const DetectorConstruction *> (G4RunManager::GetRunManager() -> GetUserDetectorConstruction());
    scoringVolume = detectorConstruction -> GetScoringVolume();

    if (arguments == 1 || arguments == 2)
    {   
        std::vector<G4LogicalVolume*> scoringVolumes = detectorConstruction -> GetAllScoringVolumes();
        if (std::find(scoringVolumes.begin(), scoringVolumes.end(), Volume) == scoringVolumes.end()) {return;}
        {
            EDep = step -> GetTotalEnergyDeposit();
            if (EDep > 0.0) {fEventAction -> AddEDep(EDep);}
            fEventAction -> AddEDep(EDep);
            // G4cout << "Energy deposition (keV): " << EDep << G4endl; 
        }
    }

    if (arguments == 3) 
    {
        if(Volume != scoringVolume) { return; }
        // G4cout << "particle touches target: " << scoringVolume << G4endl;

        endPoint = step -> GetPostStepPoint();
        processName = endPoint -> GetProcessDefinedStep() -> GetProcessName();
        Run * run = static_cast <Run *> (G4RunManager::GetRunManager() -> GetNonConstCurrentRun()); 
        run -> CountProcesses(processName);

        G4RunManager::GetRunManager() -> AbortEvent();  // kill event after first interaction
    }

    if (arguments == 4 || arguments == 5)
    {   
        std::vector<G4LogicalVolume*> scoringVolumes = detectorConstruction -> GetAllScoringVolumes();
        if (std::find(scoringVolumes.begin(), scoringVolumes.end(), Volume) == scoringVolumes.end()) {return;}
        {       
            EDep = step -> GetTotalEnergyDeposit();
            if (EDep > 0.0) {fEventAction -> AddEDep(EDep);}
            // G4cout << "Energy deposition (eV): " << G4BestUnit(EDep, "Energy") << G4endl; 
        }
    }
}

// minStepSize = 1.0e-4*mm;
// stepLength = step -> GetStepLength(); // G4cout << "Step Lenght: " << stepLength/mm << G4endl;
// currentPhysVolume = step -> GetPreStepPoint() -> GetPhysicalVolume();
// if (currentPhysVolume -> GetName() == "PhysicalWorld" && stepLength < minStepSize && stepLength != 0)
// {
//     track -> SetTrackStatus(fStopAndKill);
//     std::cout << "### Stuck Particle in " << currentPhysVolume -> GetName() <<", Killed ###" << std::endl;
// }

// const G4double threshold = 1.0e-6; 

// G4VPhysicalVolume * currentVolume = step->GetPreStepPoint()->GetPhysicalVolume();
// G4int trackID = track -> GetTrackID();
// G4ThreeVector currentPosition = track -> GetPosition();

// if (currentVolume -> GetName() == "PhysicalWorld") 
// {
//     if (stuckParticles.find(trackID) != stuckParticles.end()) // Check if this track is already being monitored
//     {
//         ParticleData & data = stuckParticles[trackID];

//         if ((currentPosition - data.lastPosition).mag() < threshold) 
//         {
//             // Increment stuck step count
//             data.stuckStepCount++;

//             if (data.stuckStepCount >= 5) 
//             {
//                 G4cout << "Particle " << trackID << " is stuck for 5 steps at position: " << currentPosition << G4endl;
//                 track->SetTrackStatus(fStopAndKill);
//             }
//         } 
//         else {data.stuckStepCount = 0;}
//         data.lastPosition = currentPosition;
//     } 
//     else {stuckParticles[trackID] = {currentPosition, 0};} // Add new track to monitoring
// }