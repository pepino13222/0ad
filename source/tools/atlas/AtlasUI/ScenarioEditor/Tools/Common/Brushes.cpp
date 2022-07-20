/* Copyright (C) 2015 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"

#include "Brushes.h"

#include "GameInterface/Messages.h"

#include "wx/spinctrl.h"

Brush g_Brush_Elevation; // shared between several elevation-related tools; other tools have their own brushes

static Brush* g_Brush_CurrentlyActive = NULL; // only one brush can be active at once

const float Brush::STRENGTH_MULTIPLIER = 1024.f;

Brush::Brush()
: m_IsActive(false)
{
	shape.reset(new BrushCircle(16, 1.f));
}

Brush::~Brush()
{
	// Avoid dangling pointers
	if (g_Brush_CurrentlyActive == this)
		g_Brush_CurrentlyActive = NULL;
}

int Brush::GetWidth() const
{
	return shape->getWidth();
}

int Brush::GetHeight() const
{
	return shape->getHight();
}

void Brush::MakeActive()
{
	if (g_Brush_CurrentlyActive)
		g_Brush_CurrentlyActive->m_IsActive = false;

	g_Brush_CurrentlyActive = this;
	m_IsActive = true;

	Send();
}

void Brush::Send()
{
	if (m_IsActive)
		POST_MESSAGE(Brush, (shape->getWidth(),shape->getHight(), shape->GetData()));
}

void Brush::SetCircle(int size)
{
	float str = shape->getStrenght();
	shape.reset(new BrushCircle(size,str));
}

void Brush::SetPyramid(int size)
{
	float str = shape->getStrenght();
	shape.reset(new BrushPyramid(size, str));
}

void Brush::SetSquare(int size)
{
	float str = shape->getStrenght();
	shape.reset(new BrushSquare(size, str));
}

void Brush::SetRidge(int size)
{
	float str = shape->getStrenght();
	shape.reset(new BrushRidge(size, str));
}
void Brush::SetX(int size)
{
	float str = shape->getStrenght();
	shape.reset(new BrushX(size, str));
}

float Brush::GetStrength() const
{
	return shape->getStrenght();
}

void Brush::SetStrength(float strength)
{
	shape->setStrenght(strength);
}

//////////////////////////////////////////////////////////////////////////

class BrushShapeCtrl : public wxRadioBox
{
public:
	BrushShapeCtrl(wxWindow* parent, wxArrayString& shapes, Brush& brush)
		: wxRadioBox(parent, wxID_ANY, _("Shapes"), wxDefaultPosition, wxDefaultSize, shapes, 0, wxRA_SPECIFY_ROWS),
		m_Brush(brush)
	{
		SetSelection(0);
	}

private:
	Brush& m_Brush;

	void OnChange(wxCommandEvent& WXUNUSED(evt))
	{
		if (GetSelection() == 0) {
			m_Brush.SetCircle(m_Brush.shape->getSize());
		}
		else if(GetSelection() == 1) {
			m_Brush.SetSquare(m_Brush.shape->getSize());
		}
		else if (GetSelection() == 2) {
			m_Brush.SetPyramid(m_Brush.shape->getSize());
		}
		else if (GetSelection() == 3) {
			m_Brush.SetRidge(m_Brush.shape->getSize());
		}
		else if (GetSelection() == 4) {
			m_Brush.SetX(m_Brush.shape->getSize());
		}
		m_Brush.Send();
	}
	DECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(BrushShapeCtrl, wxRadioBox)
	EVT_RADIOBOX(wxID_ANY, BrushShapeCtrl::OnChange)
END_EVENT_TABLE()


class BrushSizeCtrl: public wxSpinCtrl
{
public:
	BrushSizeCtrl(wxWindow* parent, Brush& brush)
		: wxSpinCtrl(parent, wxID_ANY, wxString::Format(_T("%d"), brush.shape->getSize()), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, brush.shape->getSize()),
		m_Brush(brush)
	{
	}

private:
	Brush& m_Brush;

	void OnChange(wxSpinEvent& WXUNUSED(evt))
	{
		m_Brush.shape->setSize(GetValue());
		m_Brush.Send();
	}

	DECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(BrushSizeCtrl, wxSpinCtrl)
	EVT_SPINCTRL(wxID_ANY, BrushSizeCtrl::OnChange)
END_EVENT_TABLE()


class BrushStrengthCtrl : public wxSpinCtrl
{
public:
	BrushStrengthCtrl(wxWindow* parent, Brush& brush)
		: wxSpinCtrl(parent, wxID_ANY, wxString::Format(_T("%d"), (int)(10.f*brush.shape->getStrenght())), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, (int)(10.f*brush.shape->getStrenght())),
		m_Brush(brush)
	{
	}

private:
	Brush& m_Brush;

	void OnChange(wxSpinEvent& WXUNUSED(evt))
	{
		
		m_Brush.shape->setStrenght(GetValue() / 10.f);
		m_Brush.Send();
	}

	DECLARE_EVENT_TABLE();
};
BEGIN_EVENT_TABLE(BrushStrengthCtrl, wxSpinCtrl)
	EVT_SPINCTRL(wxID_ANY, BrushStrengthCtrl::OnChange)
END_EVENT_TABLE()



void Brush::CreateUI(wxWindow* parent, wxSizer* sizer)
{
	wxArrayString shapes; // Must match order of BrushShape enum
	shapes.Add(_("Circle"));
	shapes.Add(_("Square"));
	shapes.Add(_("Pyramid"));
	shapes.Add(_("Ridge"));
	// TODO (maybe): get rid of the extra static box, by not using wxRadioBox
	sizer->Add(new BrushShapeCtrl(parent, shapes, *this), wxSizerFlags().Expand());

	sizer->AddSpacer(5);

	// TODO: These are yucky
	wxFlexGridSizer* spinnerSizer = new wxFlexGridSizer(2, 5, 5);
	spinnerSizer->AddGrowableCol(1);
	spinnerSizer->Add(new wxStaticText(parent, wxID_ANY, _("Size")), wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT));
	spinnerSizer->Add(new BrushSizeCtrl(parent, *this), wxSizerFlags().Expand());
	spinnerSizer->Add(new wxStaticText(parent, wxID_ANY, _("Strength")), wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT));
	spinnerSizer->Add(new BrushStrengthCtrl(parent, *this), wxSizerFlags().Expand());
	sizer->Add(spinnerSizer, wxSizerFlags().Expand());
}
