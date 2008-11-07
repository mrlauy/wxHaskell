#include "wrapper.h"
#include <wx/xrc/xmlres.h>

#ifdef wxUSE_STC
# include "wx/stc/stc.h"
#endif

class wxMDIParentFrameXmlHandler : public wxXmlResourceHandler
{
public:
    wxMDIParentFrameXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

class wxMDIChildFrameXmlHandler : public wxXmlResourceHandler
{
public:
    wxMDIChildFrameXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

class wxSplitterWindowXmlHandler : public wxXmlResourceHandler
{
public:
    wxSplitterWindowXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#ifdef wxUSE_STC
class wxStyledTextCtrlXmlHandler : public wxXmlResourceHandler
{
public:
    wxStyledTextCtrlXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};
#endif

class wxGridXmlHandler : public wxXmlResourceHandler
{
public:
    wxGridXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

wxMDIParentFrameXmlHandler::wxMDIParentFrameXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxDEFAULT_FRAME_STYLE);
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);

    XRC_ADD_STYLE(wxFRAME_TOOL_WINDOW);
    XRC_ADD_STYLE(wxFRAME_FLOAT_ON_PARENT);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxSTAY_ON_TOP);

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxCLIP_CHILDREN);

    AddWindowStyles();
}

wxObject *wxMDIParentFrameXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxMDIParentFrame);

    frame->Create(m_parentAsWindow,
                  GetID(),
                  GetText(wxT("title")),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), wxDEFAULT_FRAME_STYLE),
                  GetName());

    if (HasParam(wxT("size")))
        frame->SetClientSize(GetSize());
    if (HasParam(wxT("pos")))
        frame->Move(GetPosition());

    SetupWindow(frame);

    CreateChildren(frame);

    if (GetBool(wxT("centered"), FALSE))
        frame->Centre();

    return frame;
}

bool wxMDIParentFrameXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMDIParentFrame"));
}

wxMDIChildFrameXmlHandler::wxMDIChildFrameXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxDEFAULT_FRAME_STYLE);
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);

    XRC_ADD_STYLE(wxFRAME_TOOL_WINDOW);
    XRC_ADD_STYLE(wxFRAME_FLOAT_ON_PARENT);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxSTAY_ON_TOP);

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxCLIP_CHILDREN);

    AddWindowStyles();
}

wxObject *wxMDIChildFrameXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxMDIChildFrame);
	
	wxMDIParentFrame* prt = wxDynamicCast (m_parentAsWindow, wxMDIParentFrame);

	if (prt == NULL)
	{
		wxLogError(wxT("Error in resource: wxMDIChildFrame has no wxMDIParentFrame."));
		return NULL;
	}

    frame->Create(prt,
                  GetID(),
                  GetText(wxT("title")),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), wxDEFAULT_FRAME_STYLE),
                  GetName());

    SetupWindow(frame);

    CreateChildren(frame);

    if (GetBool(wxT("centered"), FALSE))
        frame->Centre();

    return frame;
}

bool wxMDIChildFrameXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMDIChildFrame"));
}

wxSplitterWindowXmlHandler::wxSplitterWindowXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSP_3D);
    XRC_ADD_STYLE(wxSP_3DSASH);
    XRC_ADD_STYLE(wxSP_BORDER);
    XRC_ADD_STYLE(wxSP_NOBORDER);
    XRC_ADD_STYLE(wxSP_PERMIT_UNSPLIT);
    XRC_ADD_STYLE(wxSP_LIVE_UPDATE);

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxCLIP_CHILDREN);

    AddWindowStyles();
}

wxObject *wxSplitterWindowXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxSplitterWindow);

    frame->Create(m_parentAsWindow,
                  GetID(),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), wxSP_3D),
                  GetName());

    SetupWindow(frame);

    CreateChildren(frame);
	
	if (frame->GetChildren().GetCount() != 2)
	{
		wxLogError(wxT("Error in resource: Splitter window needs exactly two children."));
		return NULL;
	}
	
	frame->SetSplitMode(GetLong (wxT("splitmode"), wxSPLIT_VERTICAL));
	long sashpos = GetLong (wxT("sashposition"), 100);
	
	wxWindowList::Node* node = frame->GetChildren().GetFirst();
	wxWindow* wnd1 = node->GetData();
	wxWindow* wnd2 = node->GetNext()->GetData();

	if (frame->GetSplitMode() == wxSPLIT_VERTICAL)
		frame->SplitVertically (wnd1, wnd2, sashpos);
	else
		frame->SplitHorizontally (wnd1, wnd2, sashpos);
	
    return frame;
}

bool wxSplitterWindowXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxSplitterWindow"));
}

#ifdef wxUSE_STC
wxStyledTextCtrlXmlHandler::wxStyledTextCtrlXmlHandler() : wxXmlResourceHandler()
{
    AddWindowStyles();
}

wxObject *wxStyledTextCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxStyledTextCtrl);
	
    frame->Create(m_parentAsWindow,
                  GetID(),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), 0),
                  GetName());

    if (HasParam(wxT("size")))
        frame->SetSize(GetSize());
    if (HasParam(wxT("pos")))
        frame->Move(GetPosition());

    SetupWindow(frame);

    return frame;
}

bool wxStyledTextCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStyledTextCtrl"));
}
#endif

wxGridXmlHandler::wxGridXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxCLIP_CHILDREN);

    AddWindowStyles();
}

wxObject *wxGridXmlHandler::DoCreateResource()
{
	wxGrid* grid = new wxGrid(m_parentAsWindow,
                              GetID(),
                              wxDefaultPosition, wxDefaultSize,
                              GetStyle(wxT("style"), wxWANTS_CHARS),
                              GetName());

	long cols = GetLong (wxT("numcols"), 0);
	long rows = GetLong (wxT("numrows"), 0);
	
	if (cols && rows)
		grid->CreateGrid(cols, rows, (wxGrid::wxGridSelectionModes)GetLong (wxT("selmode"), 0));
	
    if (HasParam(wxT("size")))
        grid->SetSize(GetSize());
    if (HasParam(wxT("pos")))
        grid->Move(GetPosition());

	SetupWindow(grid);

	return grid;
}

bool wxGridXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGrid"));
}

extern "C"
{

EWXWEXPORT(bool,wxXmlResource_Load)(wxXmlResource* _obj, wxString* filemask)
{
	wxGetApp().InitZipFileSystem();
	return _obj->Load(*filemask);
}
	
EWXWEXPORT(void,wxXmlResource_InitAllHandlers)(wxXmlResource* _obj)
{
	_obj->InitAllHandlers();
	_obj->AddHandler(new wxMDIParentFrameXmlHandler());
	_obj->AddHandler(new wxMDIChildFrameXmlHandler());
	_obj->AddHandler(new wxSplitterWindowXmlHandler());
#ifdef wxUSE_STC
	_obj->AddHandler(new wxStyledTextCtrlXmlHandler());
#endif
	_obj->AddHandler(new wxGridXmlHandler());
}
	
EWXWEXPORT(wxXmlResource*, wxXmlResource_Create)(int flags)
{
	wxXmlResource* _obj = wxXmlResource::Get();

	// Calling the wxc variant of InitAllHandlers() ensures additional
	// handlers for splitters etc. get initialized as well.
	wxXmlResource_InitAllHandlers(_obj);
	_obj->SetFlags(flags);
	return _obj;
}
	
EWXWEXPORT(wxXmlResource*, wxXmlResource_CreateFromFile)(wxString* filemask, int flags)
{
	wxXmlResource* _obj = wxXmlResource_Create(flags);
    if (_obj->Load(*filemask)) {
		return _obj;
    }
	else {
		delete _obj;
		return NULL;
	}
}
	
EWXWEXPORT(void,wxXmlResource_AddHandler)(wxXmlResource* _obj, wxXmlResourceHandler* handler)
{
	_obj->AddHandler(handler);
}
	
EWXWEXPORT(void,wxXmlResource_InsertHandler)(wxXmlResource* _obj, wxXmlResourceHandler* handler)
{
	_obj->InsertHandler(handler);
}
	
EWXWEXPORT(void,wxXmlResource_ClearHandlers)(wxXmlResource* _obj)
{
	_obj->ClearHandlers();
}
	
EWXWEXPORT(void,wxXmlResource_AddSubclassFactory)(wxXmlResource* _obj, wxXmlSubclassFactory* factory)
{
	_obj->AddSubclassFactory(factory);
}

EWXWEXPORT(wxMenu*,wxXmlResource_LoadMenu)(wxXmlResource* _obj, wxString* name)
{
	return _obj->LoadMenu(*name);
}
	
EWXWEXPORT(wxMenuBar*,wxXmlResource_LoadMenuBar)(wxXmlResource* _obj, wxWindow* parent, wxString* name)
{
	return _obj->LoadMenuBar(parent, *name);
}
	
EWXWEXPORT(wxToolBar*,wxXmlResource_LoadToolBar)(wxXmlResource* _obj, wxWindow* parent, wxString* name)
{
	return _obj->LoadToolBar(parent, *name);
}
	
EWXWEXPORT(wxDialog*,wxXmlResource_LoadDialog)(wxXmlResource* _obj, wxWindow* parent, wxString* name)
{
    wxDialog* dlg = new wxDialog;
    if (!_obj->LoadDialog(dlg, parent, *name)) {
        delete dlg;
        return NULL;
    } else {
        return dlg;
    }
}
	
EWXWEXPORT(wxPanel*,wxXmlResource_LoadPanel)(wxXmlResource* _obj, wxWindow* parent, wxString* name)
{
	return _obj->LoadPanel(parent, *name);
}
	
EWXWEXPORT(wxFrame*,wxXmlResource_LoadFrame)(wxXmlResource* _obj, wxWindow* parent, wxString* name)
{
    wxFrame* frame = new wxFrame;
    if (!_obj->LoadFrame(frame, parent, *name)) {
        delete frame;
        return NULL;
    } else {
        return frame;
    }
}
	
EWXWEXPORT(void,wxXmlResource_LoadBitmap)(wxXmlResource* _obj, wxString* name, void* _ref)
{
	*((wxBitmap*)_ref) = _obj->LoadBitmap(*name);
}
	
EWXWEXPORT(void,wxXmlResource_LoadIcon)(wxXmlResource* _obj, wxString* name, void* _ref)
{
	*((wxIcon*)_ref) = _obj->LoadIcon(*name);
}
	
EWXWEXPORT(bool,wxXmlResource_Unload)(wxXmlResource* _obj, wxString* name)
{
	return _obj->Unload(*name);
}
	
EWXWEXPORT(bool,wxXmlResource_AttachUnknownControl)(wxXmlResource* _obj, wxString* name, wxWindow* control, wxWindow* parent)
{
	return _obj->AttachUnknownControl(*name, control, parent);
}
	
EWXWEXPORT(int,wxXmlResource_GetXRCID)(wxXmlResource* _obj, wxString* str_id)
{
	return _obj->GetXRCID(*str_id);
}
	
EWXWEXPORT(long,wxXmlResource_GetVersion)(wxXmlResource* _obj)
{
	return _obj->GetVersion();
}
	
EWXWEXPORT(int,wxXmlResource_CompareVersion)(wxXmlResource* _obj, int major, int minor, int release, int revision)
{
	return _obj->CompareVersion(major, minor, release, revision);
}
	
EWXWEXPORT(wxXmlResource*,wxXmlResource_Get)(wxXmlResource* _obj)
{
  return wxXmlResource::Get();
}

// BUILD_XRCGETCTRL_FN constructs functions for geting control pointers out of 
// window hierarchies created from XRC files. The functions themselves 
#define BUILD_XRCGETCTRL_FN(_typ)                                                            \
  EWXWEXPORT(wx##_typ *, wxXmlResource_Get##_typ)(wxWindow* _win, wxString* _str_id)          \
  {                                                                                          \
  return reinterpret_cast<wx##_typ *>(_win->FindWindow(wxXmlResource::GetXRCID(*_str_id))); \
  }
// Construct the XRC control getter functions
BUILD_XRCGETCTRL_FN(Sizer)
BUILD_XRCGETCTRL_FN(BoxSizer)
BUILD_XRCGETCTRL_FN(StaticBoxSizer)
BUILD_XRCGETCTRL_FN(GridSizer)
BUILD_XRCGETCTRL_FN(FlexGridSizer)
BUILD_XRCGETCTRL_FN(BitmapButton)
BUILD_XRCGETCTRL_FN(Button)
BUILD_XRCGETCTRL_FN(CalendarCtrl)
BUILD_XRCGETCTRL_FN(CheckBox)
BUILD_XRCGETCTRL_FN(CheckListBox)
BUILD_XRCGETCTRL_FN(Choice)
BUILD_XRCGETCTRL_FN(ComboBox)
BUILD_XRCGETCTRL_FN(Gauge)
BUILD_XRCGETCTRL_FN(Grid)
BUILD_XRCGETCTRL_FN(HtmlWindow)
BUILD_XRCGETCTRL_FN(ListBox)
BUILD_XRCGETCTRL_FN(ListCtrl)
BUILD_XRCGETCTRL_FN(MDIChildFrame)
BUILD_XRCGETCTRL_FN(MDIParentFrame)
BUILD_XRCGETCTRL_FN(Menu)
BUILD_XRCGETCTRL_FN(MenuBar)
BUILD_XRCGETCTRL_FN(MenuItem)
BUILD_XRCGETCTRL_FN(Notebook)
BUILD_XRCGETCTRL_FN(Panel)
BUILD_XRCGETCTRL_FN(RadioButton)
BUILD_XRCGETCTRL_FN(RadioBox)
BUILD_XRCGETCTRL_FN(ScrollBar)
BUILD_XRCGETCTRL_FN(ScrolledWindow)
BUILD_XRCGETCTRL_FN(Slider)
BUILD_XRCGETCTRL_FN(SpinButton)
BUILD_XRCGETCTRL_FN(SpinCtrl)
BUILD_XRCGETCTRL_FN(SplitterWindow)
#ifdef wxUSE_STC
BUILD_XRCGETCTRL_FN(StyledTextCtrl)
#else
EWXWEXPORT(void*, wxXmlResource_GetStyledTextCtrl)(wxWindow* _win, wxString* _str_id)
{
  return NULL;
}
#endif
BUILD_XRCGETCTRL_FN(StaticBitmap)
BUILD_XRCGETCTRL_FN(StaticBox)
BUILD_XRCGETCTRL_FN(StaticLine)
BUILD_XRCGETCTRL_FN(StaticText)
BUILD_XRCGETCTRL_FN(TextCtrl)
BUILD_XRCGETCTRL_FN(TreeCtrl)
	
EWXWEXPORT(wxXmlResource*,wxXmlResource_Set)(wxXmlResource* _obj, wxXmlResource* res)
{
	return _obj->Set(res);
}

EWXWEXPORT(int,wxXmlResource_GetDomain)(wxXmlResource* _obj, wxChar* buf)
{
	buf = _obj->GetDomain();
	return wxStrlen(buf);
}

EWXWEXPORT(void,wxXmlResource_SetDomain)(wxXmlResource* _obj, wxString* domain)
{
	_obj->SetDomain(*domain);
}

EWXWEXPORT(int,wxXmlResource_GetFlags)(wxXmlResource* _obj)
{
	return _obj->GetFlags();
}
	
EWXWEXPORT(void,wxXmlResource_SetFlags)(wxXmlResource* _obj, int flags)
{
	_obj->SetFlags(flags);
}
	
}
