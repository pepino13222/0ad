/* Copyright (C) 2009 Wildfire Games.
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

#include "ScenarioEditor/ScenarioEditor.h"
#include "Common/Tools.h"
#include "Common/Brushes.h"
#include "GameInterface/Messages.h"

using AtlasMessage::Position;

class DisplacementMap : public StateDrivenTool<DisplacementMap>
{
	DECLARE_DYNAMIC_CLASS(DisplacementMap);

	int m_Direction; // +1 = raise, -1 = lower
	Position m_Pos;
	Brush brush;

public:
	DisplacementMap()
	{
		brush.SetX(5);
		brush.SetStrength(1024);
		SetState(&Waiting);
	}


	void OnEnable()
	{
		brush.MakeActive();
	}

	void OnDisable()
	{
		POST_MESSAGE(BrushPreview, (false, Position()));
	}


	struct sWaiting : public State
	{
		bool OnMouse(DisplacementMap*  obj, wxMouseEvent& evt)
		{
			if (evt.LeftDown())
			{
				Position pos(evt.GetPosition());
				obj->m_Pos = pos;
				POST_MESSAGE(BrushPreview, (true, obj->m_Pos));
				SET_STATE(Raising);
				//POST_COMMAND(FillTerrain, (pos, (std::wstring)g_SelectedTexture.wc_str()));
				return true;
			}
			
			else if (evt.Moving())
			{
				POST_MESSAGE(BrushPreview, (true, Position(evt.GetPosition())));
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	Waiting;


	struct sAltering_common : public State
	{
		void OnEnter(DisplacementMap* obj)
		{
			POST_MESSAGE(BrushPreview, (true, obj->m_Pos));
		}

		void OnLeave(DisplacementMap*)
		{
			ScenarioEditor::GetCommandProc().FinaliseLastCommand();
		}

		bool OnMouse(DisplacementMap* obj, wxMouseEvent& evt)
		{
			if (IsMouseUp(evt))
			{
				SET_STATE(Waiting);
				return true;
			}
			else if (evt.Dragging())
			{
				wxPoint pos = evt.GetPosition();
				obj->m_Pos = Position(pos);
				POST_MESSAGE(BrushPreview, (true, obj->m_Pos));
				return true;
			}
			else
			{
				return false;
			}
		}

		void OnTick(DisplacementMap* obj, float dt)
		{
			POST_COMMAND(DisplacementMap, (obj->m_Pos, dt * g_Brush_Elevation.STRENGTH_MULTIPLIER * GetDirection() * g_Brush_Elevation.GetStrength()));
			obj->m_Pos = Position::Unchanged();
		}

		virtual bool IsMouseUp(wxMouseEvent& evt) = 0;
		virtual int GetDirection() = 0;
	};

	struct sRaising : public sAltering_common
	{
		bool IsMouseUp(wxMouseEvent& evt) { return evt.LeftUp(); }
		int GetDirection() { return 1; }
	}
	Raising;

	struct sLowering : public sAltering_common
	{
		bool IsMouseUp(wxMouseEvent& evt) { return evt.RightUp(); }
		int GetDirection() { return -1; }
	}
	Lowering;
};

IMPLEMENT_DYNAMIC_CLASS(DisplacementMap, StateDrivenTool<DisplacementMap>);
