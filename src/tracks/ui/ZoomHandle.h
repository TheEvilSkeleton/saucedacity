/**********************************************************************

Audacity: A Digital Audio Editor

ZoomHandle.h

Paul Licameli split from TrackPanel.cpp

**********************************************************************/

#ifndef __AUDACITY_ZOOM_HANDLE__
#define __AUDACITY_ZOOM_HANDLE__

#include "../../UIHandle.h"

class wxMouseState;

// This handle class, unlike most, doesn't associate with any particular cell.
class ZoomHandle final : public UIHandle
{
   ZoomHandle(const ZoomHandle&) = delete;
   static HitTestPreview HitPreview
      (const wxMouseState &state, const SaucedacityProject *pProject);

public:
   ZoomHandle();

   ZoomHandle &operator=(const ZoomHandle&) = default;

   static UIHandlePtr HitAnywhere
      (std::weak_ptr<ZoomHandle> &holder);
   static UIHandlePtr HitTest
      (std::weak_ptr<ZoomHandle> &holder, const wxMouseState &state);

   virtual ~ZoomHandle();

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

   bool IsDragZooming() const;

   int mZoomStart{}, mZoomEnd{};
   wxRect mRect{};
};

#endif
