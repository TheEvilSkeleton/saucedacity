/**********************************************************************

Audacity: A Digital Audio Editor

NoteTrackVZoomHandle.h

Paul Licameli split from TrackPanel.cpp

**********************************************************************/

#ifndef __AUDACITY_NOTE_TRACK_VZOOM_HANDLE__
#define __AUDACITY_NOTE_TRACK_VZOOM_HANDLE__

class wxMouseState;
class NoteTrack;

#include "../../../../UIHandle.h"

class NoteTrackVZoomHandle : public UIHandle
{
   NoteTrackVZoomHandle(const NoteTrackVZoomHandle&);
   static HitTestPreview HitPreview(const wxMouseState &state);

public:
   explicit NoteTrackVZoomHandle
      (const std::shared_ptr<NoteTrack> &pTrack, const wxRect &rect, int y);

   NoteTrackVZoomHandle &operator=(const NoteTrackVZoomHandle&) = default;

   static UIHandlePtr HitTest
      (std::weak_ptr<NoteTrackVZoomHandle> &holder,
       const wxMouseState &state,
       const std::shared_ptr<NoteTrack> &pTrack, const wxRect &rect);

   virtual ~NoteTrackVZoomHandle();

   std::shared_ptr<NoteTrack> GetTrack() const { return mpTrack.lock(); }

   void Enter(bool forward, SaucedacityProject *) override;

   Result Click
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject) override;

   Result Drag
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject) override;

   HitTestPreview Preview
      (const TrackPanelMouseState &state, SaucedacityProject *pProject)
      override;

   Result Release
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject,
       wxWindow *pParent) override;

   Result Cancel(SaucedacityProject *pProject) override;

private:
   // TrackPanelDrawable implementation
   void Draw(
      TrackPanelDrawingContext &context,
      const wxRect &rect, unsigned iPass ) override;

   wxRect DrawingArea(
      TrackPanelDrawingContext &,
      const wxRect &rect, const wxRect &panelRect, unsigned iPass ) override;

   std::weak_ptr<NoteTrack> mpTrack;

   int mZoomStart, mZoomEnd;
   wxRect mRect;
};

#endif
