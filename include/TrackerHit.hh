#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    TrackerHit(const TrackerHit&);
    virtual ~TrackerHit();

    const TrackerHit& operator=(const TrackerHit&);
    G4int operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    virtual void Draw();
    virtual void Print();

    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)     	{ fEdep = de; };
    void SetPos      (G4ThreeVector xyz)	{ fPos = xyz; };

    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:
      G4int         fTrackID;
      G4double      fEdep;
      G4ThreeVector fPos;
};

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator) TrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void *) TrackerHitAllocator->MallocSingle();
}

inline void TrackerHit::operator delete(void *Hit)
{
  TrackerHitAllocator->FreeSingle((TrackerHit*) Hit);
}

#endif

