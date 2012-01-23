#ifndef ALIVANALYSISMUON_H
#define ALIVANALYSISMUON_H

/* $Id: AliVAnalysisMuon.h 47782 2011-02-24 18:37:31Z martinez $ */ 

//
// Base class for single muon analysis
//
// Author: Diego Stocco
//

#include "AliAnalysisTaskSE.h"
#include "AliMuonTrackCuts.h"
#include "AliMuonPairCuts.h"

class TString;
class TObjArray;
class TAxis;
class TLorentzVector;
class TList;
class AliMergeableCollection;
class AliCounterCollection;
class AliVParticle;
class AliAODEvent;
class AliESDEvent;
class AliCFGridSparse;

class AliVAnalysisMuon : public AliAnalysisTaskSE {
 public:
  AliVAnalysisMuon();
  AliVAnalysisMuon(const char *name, const AliMuonTrackCuts& trackCuts);
  AliVAnalysisMuon(const char *name, const AliMuonPairCuts& pairCuts);
  AliVAnalysisMuon(const char *name, const AliMuonTrackCuts& trackCuts, const AliMuonPairCuts& pairCuts);
  virtual ~AliVAnalysisMuon();

  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *option);
  virtual void   NotifyRun();
  virtual void   FinishTaskOutput();

  void SetCentralityClasses(Int_t nCentralityBins = -1, Double_t* centralityBins = 0x0);

  void SetTrigClassPatterns(TString pattern = "CINT CMU !CMUP CMBAC CPBI !-ACE- !-AC- !-E- !WU !EGA !EJE");
  void SetTerminateOptions(TString physSel="All", TString trigClass="ANY", TString centralityRange="", TString furtherOpts="");
  
  // Utility methods for CF container
  static Bool_t SetSparseRange(AliCFGridSparse* gridSparse,
                               Int_t ivar, TString labelName,
                               Double_t varMin, Double_t varMax,
                               TString option = "");

 protected:
  
  /////////////////////////////////////////////////////
  // Pure virtual methods to be implemented bu users //
  /////////////////////////////////////////////////////
  
  virtual void MyUserCreateOutputObjects() = 0;
  // In this method you have to create your own output as well as
  // the mergeable objects that will be then used
  // in the counter collection.
  // To do so, create your object and add it to the collection through:
  //    TH1* histo = new TH1F();
  //    AddObjectToCollection(histo, index)
  
  virtual void ProcessEvent(TString physSel, const TObjArray& selectTrigClasses, TString centrality) = 0;
  // This method is called at each event.
  // In this method you can fill the histograms or the CF container that you have created
  
  /////////////////////
  // Utility methods //
  /////////////////////
  
  // Transparently handle tracks in ESD/AOD
  Int_t GetNTracks();
  AliVParticle* GetTrack(Int_t itrack);
  TLorentzVector GetTrackPair(AliVParticle* track1, AliVParticle* track2) const;
  
  // Methods for MC
  Bool_t IsMC();
  Int_t GetNMCTracks();
  AliVParticle* GetMCTrack(Int_t trackLabel);
  Int_t GetParticleType(AliVParticle* track);
  Int_t RecoTrackMother(AliVParticle* mcParticle);
  
  // Methods for mergeable object collections
  Bool_t AddObjectToCollection(TObject* object, Int_t index = -1);
  TObject* GetMergeableObject(TString physSel, TString trigClassName, TString centrality, TString objectName);
  TObject* GetSum(TString physSel, TString trigClassNames, TString centrality, TString objectPattern);
  
  // A useful constant
  Double_t MuonMass2() const;
  
  // Handle triggers
  Bool_t TrackPtCutMatchTrigClass(AliVParticle* track, TString trigClassName);
  Int_t GetTrigClassPtCutLevel(TString trigClassName);
  
  enum {
    kPhysSelPass,    ///< Physics selected events
    kPhysSelReject,  ///< Events non-passing selection
    kNselections     ///< Number of selections
  };
  
  enum {
    kCharmMu,       ///< Mu from charm
    kBeautyMu,      ///< Mu from beauty
    kQuarkoniumMu,  ///< Mu from resonance
    kDecayMu,       ///< Decay mu
    kSecondaryMu,   ///< Secondary mu
    kRecoHadron,    ///< Reconstructed hadron
    kUnidentified,  ///< Particle that fails matching kine
    kNtrackSources  ///< Total number of track sources
  };
    
  AliMuonTrackCuts fMuonTrackCuts; ///< Muon track cuts
  AliMuonPairCuts fMuonPairCuts;   ///< Muon pair track cuts
  AliESDEvent* fESDEvent;      //!< ESD event, not owner
  AliAODEvent* fAODEvent;      //!< AOD event, not owner
  TObjArray* fTerminateOptions; ///< Terminate options
  TObjArray* fChargeKeys;      ///< Muon charge keys
  TObjArray* fSrcKeys;         ///< MC sources names
  TObjArray* fPhysSelKeys;     ///< Physics selection names
  TList* fTriggerClasses;      ///< List of trigger classes
  TAxis* fCentralityClasses;   ///< Centrality classes
  
  AliCounterCollection* fEventCounters;  //!< event counters
  AliMergeableCollection* fMergeableCollection; //!< collection of mergeable objects
  TObjArray* fOutputList;  //!< List of outputs  

 private:
  AliVAnalysisMuon(const AliVAnalysisMuon&);
  AliVAnalysisMuon& operator=(const AliVAnalysisMuon&);
  
  void InitKeys();
  void CreateMergeableObjects(TString physSel, TString trigClassName, TString centrality);
  TObjArray* BuildTriggerClasses(TString firedTrigClasses);
  
  TObjArray* fSelectedTrigPattern; ///< List of triggers to be kept
  TObjArray* fRejectedTrigPattern; ///< List of triggers to be rejected
  TObjArray* fSelectedTrigLevel;   ///< Track-trigger pt cut for selected trigger class
  TObjArray* fOutputPrototypeList; //!< List of prototype object to be used in collection

  ClassDef(AliVAnalysisMuon, 1);
};

#endif
