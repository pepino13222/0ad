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

#ifndef INCLUDED_BRUSHES
#define INCLUDED_BRUSHES

class BrushShapeCtrl;
class BrushSizeCtrl;
class BrushStrengthCtrl;

#include <vector>

class BrushShape {
public:
	BrushShape() : strenght(1.f) {};
	virtual int getWidth() const = 0;
	virtual int getHight() const = 0;
    virtual std::vector <float> GetData() const = 0;

	virtual float getStrenght() { return strenght; }
	virtual void setStrenght(float strenght) { this->strenght = strenght; }
	virtual int getSize() const = 0;
	virtual void setSize(int size) = 0;
	virtual void setActive(bool isActive) { this->is_Active = isActive;}
	virtual bool getIsActive() { return is_Active; }
	virtual int getId()const = 0;
private:
	bool is_Active;
	float strenght;
};

class BrushSquare : public BrushShape {
public:
	BrushSquare(int size,float strenght):size(size)
	{
		this->setStrenght(strenght);
		this->setActive(false);
	}
	int getSize()const override
	{
		return size;
	}
	int getWidth() const override 
	{
		return this->size;
	}
	int getHight() const override 
	{
		return this->size;
	}
	void setSize(int size) override
	{
		this->size = size;
	}
	std::vector <float> GetData()const override 
	{
		int w = getWidth();
		int h = getHight();
		std::vector <float> data (h*w);
		int i = 0;
		for (int row = 0; row < h; ++row)
			for (int col = 0; col < w; ++col)
				data[i++] = 1.f;
		return data;
	}
	int getId()const override
	{
		return this->id;
	}
private:
	int size;
 const int id = 1;
};
class BrushCircle : public BrushShape {
public:
	BrushCircle(int size, float strenght) :size(size)
	{
		this->setStrenght(strenght);
		this->setActive(false);
	}
	int getSize()const override
	{
		return size;
	}
	int getWidth() const override
	{
		return this->size;
	}
	int getHight() const override
	{
		return this->size;
	}
	void setSize(int size) override
	{
		this->size = size;
	}
	std::vector <float> GetData()const override {
		int width = getWidth();
		int height = getHight();

		std::vector<float> data(width * height);
		int i = 0;
		// All calculations are done in units of half-tiles, since that
		// is the required precision
		int mid_x = size - 1;
		int mid_y = size - 1;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dist_sq = // scaled to 0 in centre, 1 on edge
					((2 * x - mid_x) * (2 * x - mid_x) +
						(2 * y - mid_y) * (2 * y - mid_y)) / (float)(size * size);
				if (dist_sq <= 1.f)
					data[i++] = (sqrtf(2.f - dist_sq) - 1.f) / (sqrt(2.f) - 1.f);
				else
					data[i++] = 0.f;
			}
		}
		return data;
	}
	 int getId()const override
	{
		return this->id;
	}
private:
 const int id = 2;
	int size;
};

class BrushPyramid : public BrushShape {
public:
	BrushPyramid(int size, float strenght) :size(size)
	{
		this->setStrenght(strenght);
		this->setActive(false);
	}
	int getSize()const override
	{
		return size;
	}
	int getWidth() const override
	{
		return this->size;
	}
	int getHight() const override
	{
		return this->size;
	}
	void setSize(int size) override
	{
		this->size = size;
	}
	std::vector <float> GetData()const override
	{
			std::vector<float> data(size * size);
			float half = float(size) / 2;
			int i = 0;
			for (int y = 0; y < size; ++y)
			{
				for (int x = 0; x < size; ++x)
				{
					float upBotom = 1 - abs(half - y) / half;
					float leftRight = 1 - abs(half - x) / half;
					if (upBotom < leftRight) data[i++] = upBotom;
					else data[i++] = leftRight;
				}
			}
			return data;
	}
	int getId()const override
	{
		return this->id;
	}
private:
	int size;
	const int id = 3;
};

class BrushRidge : public BrushShape {
public:
	BrushRidge(int size, float strenght) :size(size)
	{
		this->setStrenght(strenght);
		this->setActive(false);
	}
	int getSize()const override
	{
		return size;
	}
	int getWidth() const override
	{
		return this->size;
	}
	int getHight() const override
	{
		return this->size;
	}
	void setSize(int size) override
	{
		this->size = size;
	}
	std::vector <float> GetData()const override
	{
		std::vector<float> data(size * size);
		float half = float(size) / 2;
		int i = 0;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float upBotom = 1 - abs(half - y) / half;
				float leftRight = 1 - abs(half - x) / half;
				 data[i++] = upBotom;
			}
		}
		return data;
	}
	int getId()const override
	{
		return this->id;
	}
private:
	int size;
	const int id = 4;
};


class BrushX : public BrushShape {
public:
	BrushX(int size, float strenght) :size(size)
	{
		this->setStrenght(strenght);
		this->setActive(false);
	}
	int getSize()const override
	{
		return size;
	}
	int getWidth() const override
	{
		return this->size;
	}
	int getHight() const override
	{
		return this->size;
	}
	void setSize(int size) override
	{
		this->size = 5;
	}
	std::vector <float> GetData()const override
	{
		float data[] = {
			1.f, 0.f, 0.f, 0.f, 1.f,
			0.f, 1.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 1.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 1.f, 0.f,
			1.f, 0.f, 0.f, 0.f, 1.f,
		};
		std::vector<float> data2(25);
		for (size_t i = 0; i < 25; i++)
		{
			data2[i] = data[i];
		}
		return data2;
	}
	int getId()const override
	{
		return this->id;
	}
private:
	int size;
	const int id = 4;
};



class Brush
{
	friend class BrushShapeCtrl;
	friend class BrushSizeCtrl;
	friend class BrushStrengthCtrl;
public:
	Brush();
	~Brush();

	static const float STRENGTH_MULTIPLIER;

	int GetWidth() const;
	int GetHeight() const;
	//std::vector<float> GetData() const;

	void SetCircle(int size);
	void SetSquare(int size);
	void SetPyramid(int size);
	void SetRidge(int size);
	void SetX(int size);

	float GetStrength() const;
	void SetStrength(float strength);

	void CreateUI(wxWindow* parent, wxSizer* sizer);

	// Set this brush to be active - sends SetBrush message now, and also
	// whenever the brush is altered (until a different one is activated).
	void MakeActive();

private:
	// If active, send SetBrush message to the game
	void Send();

	//enum BrushShape { CIRCLE = 0, SQUARE};
	//BrushShape m_Shape;
	std::unique_ptr<BrushShape> shape;
	//int m_Size;
	//float m_Strength;
	bool m_IsActive;
};

extern Brush g_Brush_Elevation;

#endif // INCLUDED_BRUSHES
