/**********************************************************************

Audacity: A Digital Audio Editor

LabelTrackView.h

Paul Licameli split from class LabelTrack

**********************************************************************/

#ifndef __AUDACITY_LABEL_TRACK_VIEW__
#define __AUDACITY_LABEL_TRACK_VIEW__

#include "../../ui/CommonTrackView.h"

class LabelGlyphHandle;
class LabelTextHandle;
class LabelDefaultClickHandle;
class LabelStruct;
class LabelTrack;
struct LabelTrackEvent;
struct LabelTrackHit;
class NotifyingSelectedRegion;
class SelectedRegion;
struct TrackPanelDrawingContext;
class ZoomInfo;

class wxBitmap;
class wxCommandEvent;
class wxDC;
class wxMouseEvent;

constexpr int NUM_GLYPH_CONFIGS = 3;
constexpr int NUM_GLYPH_HIGHLIGHTS = 4;
constexpr int MAX_NUM_ROWS =80;

class wxKeyEvent;

class SAUCEDACITY_DLL_API LabelTrackView final : public CommonTrackView
{
   LabelTrackView( const LabelTrackView& ) = delete;
   LabelTrackView &operator=( const LabelTrackView& ) = delete;

   void Reparent( const std::shared_ptr<Track> &parent ) override;

public:
   enum : int { DefaultFontSize = 0 }; //system preferred
   static constexpr int TextFramePadding { 2 };
   static constexpr int TextFrameYOffset { -1 };
   static constexpr int LabelBarHeight { 6 }; 

   explicit
   LabelTrackView( const std::shared_ptr<Track> &pTrack );
   ~LabelTrackView() override;

   static LabelTrackView &Get( LabelTrack& );
   static const LabelTrackView &Get( const LabelTrack& );

   bool DoCaptureKey( SaucedacityProject &project, wxKeyEvent &event );
   bool DoKeyDown(
      SaucedacityProject &project, NotifyingSelectedRegion &sel, wxKeyEvent & event);
   bool DoChar(
      SaucedacityProject &project, NotifyingSelectedRegion &sel, wxKeyEvent & event);

   //This returns the index of the label we just added.
   int AddLabel(const SelectedRegion &region,
      const wxString &title = {},
      int restoreFocus = -1);

private:
   void BindTo( LabelTrack *pParent );
   void UnbindFrom( LabelTrack *pParent );

   std::vector<UIHandlePtr> DetailedHitTest
      (const TrackPanelMouseState &state,
       const SaucedacityProject *pProject, int currentTool, bool bMultiTool)
      override;

   unsigned CaptureKey
     (wxKeyEvent &event, ViewInfo &viewInfo, wxWindow *pParent,
      SaucedacityProject *project) override;

   unsigned KeyDown
      (wxKeyEvent &event, ViewInfo &viewInfo, wxWindow *pParent,
      SaucedacityProject *project) override;

   unsigned Char
      (wxKeyEvent &event, ViewInfo &viewInfo, wxWindow *pParent,
      SaucedacityProject *project) override;

   std::shared_ptr<TrackVRulerControls> DoGetVRulerControls() override;

   // Preserve some view state too for undo/redo purposes
   void CopyTo( Track &track ) const override;

public:
   static void DoEditLabels(
      SaucedacityProject &project, LabelTrack *lt = nullptr, int index = -1);

   static int DialogForLabelName(
      SaucedacityProject &project, const SelectedRegion& region,
      const wxString& initialValue, wxString& value);

   bool IsTextSelected( SaucedacityProject &project ) const;

private:
   void CreateCustomGlyphs();

public:
   static wxFont GetFont(const wxString &faceName, int size = DefaultFontSize);
   static void ResetFont();

   void Draw( TrackPanelDrawingContext &context, const wxRect & r ) const;

   bool CutSelectedText( SaucedacityProject &project );
   bool CopySelectedText( SaucedacityProject &project );
   bool PasteSelectedText(
      SaucedacityProject &project, double sel0, double sel1 );

   static void OverGlyph(
      const LabelTrack &track, LabelTrackHit &hit, int x, int y );

private:
   static wxBitmap & GetGlyph( int i);

   struct Index
   {
      Index();
      Index(int index);
      operator int() const;
      Index &operator =(int index);
      Index &operator ++();
      Index &operator --();

      bool IsModified() const;
      void SetModified(bool modified);

   private:
      int mIndex;
      bool mModified;
   };

public:
   struct Flags {
      int mInitialCursorPos, mCurrentCursorPos;
      Index mNavigationIndex;
      Index mTextEditIndex;
      wxString mUndoLabel;
   };

   void ResetFlags();
   Flags SaveFlags() const;
   void RestoreFlags( const Flags& flags );

   static int OverATextBox( const LabelTrack &track, int xx, int yy );

   static bool OverTextBox( const LabelStruct *pLabel, int x, int y );

private:
   static bool IsTextClipSupported();

public:
   void AddedLabel( const wxString &title, int pos );
   void DeletedLabel( int index );

private:
   //And this tells us the index, if there is a label already there.
   int GetLabelIndex(double t, double t1);

public:
   //get current cursor position,
   // relative to the left edge of the track panel
   bool CalcCursorX( SaucedacityProject &project, int * x ) const;

private:
   void CalcHighlightXs(int *x1, int *x2) const;

public:
   void ShowContextMenu( SaucedacityProject &project );

private:
   void OnContextMenu( SaucedacityProject &project, wxCommandEvent & evt);

   /// Keeps track of the currently selected label (not same as selection region)
   /// used for navigation between labels
   mutable Index mNavigationIndex{ -1 };
   /// Index of the current label text beeing edited
   mutable Index mTextEditIndex{ -1 };

   mutable wxString mUndoLabel;

   static int mIconHeight;
   static int mIconWidth;
   static int mTextHeight;

   static bool mbGlyphsReady;
   static wxBitmap mBoundaryGlyphs[NUM_GLYPH_CONFIGS * NUM_GLYPH_HIGHLIGHTS];

   static int mFontHeight;
   mutable int mCurrentCursorPos;                  /// current cursor position
   mutable int mInitialCursorPos;                  /// initial cursor position

   
   int mRestoreFocus{-2};                          /// Restore focus to this track
                                                   /// when done editing

   void ComputeTextPosition(const wxRect & r, int index) const;
   void ComputeLayout(const wxRect & r, const ZoomInfo &zoomInfo) const;
   static void DrawLines( wxDC & dc, const LabelStruct &ls, const wxRect & r);
   static void DrawGlyphs( wxDC & dc, const LabelStruct &ls, const wxRect & r,
      int GlyphLeft, int GlyphRight);
   static int GetTextFrameHeight();
   static void DrawText( wxDC & dc, const LabelStruct &ls, const wxRect & r);
   static void DrawTextBox( wxDC & dc, const LabelStruct &ls, const wxRect & r);
   static void DrawBar(wxDC& dc, const LabelStruct& ls, const wxRect& r);
   static void DrawHighlight(
      wxDC & dc, const LabelStruct &ls, int xPos1, int xPos2, int charHeight);

public:
   /// convert pixel coordinate to character position in text box
   int FindCursorPosition(int labelIndex, wxCoord xPos);
   int GetCurrentCursorPosition() const { return mCurrentCursorPos; }
   void SetCurrentCursorPosition(int pos);
   int GetInitialCursorPosition() const { return mInitialCursorPos; }

   /// Sets the label with specified index for editing,
   /// optionaly selection may be specified with [start, end]
   void SetTextSelection(int labelIndex, int start = 1, int end = 1);
   int GetTextEditIndex(SaucedacityProject& project) const;
   void ResetTextSelection();

   void SetNavigationIndex(int index);
   int GetNavigationIndex(SaucedacityProject& project) const;

private:

   // TrackPanelDrawable implementation
   void Draw(
      TrackPanelDrawingContext &context,
      const wxRect &rect, unsigned iPass ) override;

   static void calculateFontHeight(wxDC & dc);

   bool IsValidIndex(const Index& index, SaucedacityProject& project) const;

private:
   void RemoveSelectedText();

   void OnLabelAdded( LabelTrackEvent& );
   void OnLabelDeleted( LabelTrackEvent& );
   void OnLabelPermuted( LabelTrackEvent& );
   void OnSelectionChange( LabelTrackEvent& );

   std::shared_ptr<LabelTrack> FindLabelTrack();
   std::shared_ptr<const LabelTrack> FindLabelTrack() const;

   std::weak_ptr<LabelGlyphHandle> mGlyphHandle;
   std::weak_ptr<LabelTextHandle> mTextHandle;

   static wxFont msFont;

   // Bug #2571: See explanation in ShowContextMenu()
   int mEditIndex;
};

#endif
