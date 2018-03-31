#include "Graph.h"
#include "Simple_window.h"
#include <random>

#define PI 3.14159265

inline int rand_int(int, int);

class Octagon : public Shape
{
public:
	Octagon(Point centre, int side_length);

	void draw_lines() const;
	void modify(Point, int);

	void set_centre(Point cent) { mid = cent; modify(cent, s_l); }
	void set_side(int ex) { s_l = ex; modify(mid, ex); }

	double cot(double x) const { return (cos(x) / sin(x)); }
	double rad(int side) { return (0.5*side*cot(PI / 8)); }

private:
	int s_l;
	Point mid;
};

void Octagon::modify(Point mid, int s_l)
{
	int pos = 0;
	for (double ang = 22.5; ang <= 337.5; ang += 45)
	{
		set_point(pos++, Point{
			int(mid.x + rad(s_l) * cos(ang*PI / 180))
			,   int(mid.y - rad(s_l) * sin(ang*PI / 180))
			});
	};

}

Octagon::Octagon(Point centre, int side_length)
	:mid(centre), s_l(side_length)
{
	for (int ang = 22.5; ang <= 337.5; ang += 45)
	{
		add(Point{ int(mid.x + rad(s_l) * cos(ang*PI / 180)),
			int(mid.y - rad(s_l) * sin(ang*PI / 180)) });
	};
}

void Octagon::draw_lines() const
{
	if (color().visibility())
	{
		for (unsigned int i = 1; i<number_of_points(); ++i)
		{
			fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
		}
		fl_line(point(number_of_points() - 1).x, point(number_of_points() - 1).y,
			point(0).x, point(0).y);
	}
}


struct Lines_window : Window {
	Lines_window(Point xy, int w, int h, const string& title);
private:
	// data:
	Open_polyline lines;
	// widgets:
	Button next_button; // add (next_x,next_y) to lines
	Button quit_button; // end program
	In_box next_x;
	In_box next_y;
	Out_box xy_out;
	Menu color_menu;
	Button menu_button;
	Menu style_menu;
	Button l_style;
	void change(Color c) { lines.set_color(c); }
	void change_lstyle(Line_style l) { lines.set_style(l); }
	void hide_menu() { color_menu.hide(); menu_button.show(); }
	void hide_line_menu() { style_menu.hide(); l_style.show(); }
	// actions invoked by callbacks:
	void style() { l_style.hide(); style_menu.show(); }
	void red_pressed() { change(Color::red); hide_menu(); }
	void blue_pressed() { change(Color::blue); hide_menu(); }
	void black_pressed() { change(Color::black); hide_menu(); }
	void menu_pressed() { menu_button.hide(); color_menu.show(); }

	void dash_pressed() { change_lstyle(Line_style::dash); hide_line_menu(); }
	void dot_pressed() { change_lstyle(Line_style::dot); hide_line_menu(); }
	void dashdot_pressed() { change_lstyle(Line_style::dashdot); hide_line_menu(); }
	void dashdotdot_pressed() { change_lstyle(Line_style::dashdotdot); hide_line_menu(); }
	void solid_pressed() { change_lstyle(Line_style::solid); hide_line_menu(); }
	void next();
	void quit() { hide(); }
	// callback functions:
	static void cb_lstyle(Address, Address pw) { reference_to<Lines_window>(pw).style(); }
	static void cb_red(Address, Address pw) { reference_to<Lines_window>(pw).red_pressed(); }
	static void cb_blue(Address, Address pw) { reference_to<Lines_window>(pw).blue_pressed(); }
	static void cb_black(Address, Address pw) { reference_to<Lines_window>(pw).black_pressed(); }
	static void cb_menu(Address, Address pw) { reference_to<Lines_window>(pw).menu_pressed(); }
	static void cb_next(Address, Address pw) { reference_to<Lines_window>(pw).next(); }
	static void cb_quit(Address, Address pw) { reference_to<Lines_window>(pw).quit(); }

	
};

void Lines_window::next()
{
	int x = next_x.get_int();
	int y = next_y.get_int();

	lines.add(Point{ x,y });
	// update current position readout:
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());
	redraw();

}

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	:Window{ xy,w,h,title },
	next_button{ Point{ x_max() - 150,0 }, 70, 20, "Next point", cb_next },
	quit_button{ Point{ x_max() - 70,0 }, 70, 20, "Quit", cb_quit },
	next_x{ Point{ x_max() - 310,0 }, 50, 20, "next x:" },
	next_y{ Point{ x_max() - 210,0 }, 50, 20, "next y:" },
	xy_out{ Point{ 100,0 }, 100, 20, "current (x,y):" },
	color_menu{ Point{ x_max() - 70,30 },70,20,Menu::vertical,"color" },
	menu_button{ Point{ x_max() - 80,30 }, 80, 20, "color menu", cb_menu },
	l_style{ Point{x_max() -80, 120}, 80, 20, "line style", cb_lstyle},
	style_menu{ Point{x_max()- 70, 120}, 70, 20, Menu::vertical, "lstyle" }
{
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);
	xy_out.put("no point");
	
	color_menu.attach(new Button{ Point{ 0,0 },0,0,"red",cb_red });
	color_menu.attach(new Button{ Point{ 0,0 },0,0,"blue",cb_blue });
	color_menu.attach(new Button{ Point{ 0,0 },0,0,"black",cb_black });
	
	style_menu.attach(new Button{ Point{0,0},0,0, "dash", [](Address, Address pw) {reference_to<Lines_window>(pw).dash_pressed(); } });
	style_menu.attach(new Button{ Point{ 0,0 },0,0, "dot", [](Address, Address pw) {reference_to<Lines_window>(pw).dot_pressed(); } });
	style_menu.attach(new Button{ Point{ 0,0 },0,0, "dashdot", [](Address, Address pw) {reference_to<Lines_window>(pw).dashdot_pressed(); } });
	style_menu.attach(new Button{ Point{ 0,0 },0,0, "dashdotdot", [](Address, Address pw) {reference_to<Lines_window>(pw).dashdotdot_pressed(); } });
	style_menu.attach(new Button{ Point{ 0,0 },0,0, "solid",[](Address, Address pw) {reference_to<Lines_window>(pw).solid_pressed(); } });

	attach(style_menu);
	style_menu.hide();

	attach(l_style);


	attach(color_menu);
	color_menu.hide();
	attach(menu_button);
	attach(lines);
}

class My_window : public Window
{
public:
	My_window(Point xy, int w, int h, const string& s);

private:
	Button next_b;
	Button quit_b;
	Menu checker_c1;
	Menu checker_c2;
	Menu checker_c3;
	Menu checker_c4;


	Vector_ref<Graph_lib::Rectangle>rect;
	void init_rect();
	
	void change_color(int i,  Color c)
	{
		rect[i-1].set_fill_color(c);
	}

	
	
	void next() { show(); };
	void quit() { hide(); };
};

void My_window::init_rect()
{
	for (int i = 1; i < 5; ++i)
	{
		rect.push_back(new Graph_lib::Rectangle{ Point{100, i*100}, 100,100 });
		rect[rect.size() - 1].set_fill_color(Color::blue);
	}
	for (int i = 1; i < 5; ++i)
	{
		rect.push_back(new Graph_lib::Rectangle{ Point{ 200, i * 100 }, 100,100 });
		rect[rect.size() - 1].set_fill_color(Color::white);
	}
	for (int i = 1; i < 5; ++i)
	{
		rect.push_back(new Graph_lib::Rectangle{ Point{ 300, i * 100 }, 100,100 });
		rect[rect.size() - 1].set_fill_color(Color::red);
	}
	for (int i = 1; i < 5; ++i)
	{
		rect.push_back(new Graph_lib::Rectangle{ Point{ 400, i * 100 }, 100,100 });
		rect[rect.size() - 1].set_fill_color(Color::yellow);
	}
}



My_window::My_window(Point xy, int w, int h, const string& s)
	:Window{xy, w, h, s},
	next_b{ Point{x_max() - 100, 0}, 70 , 20 ,"Next", [](Address, Address pw)
		   {reference_to<My_window>(pw).next(); } },
	quit_b{ Point{x_max() - 100, y_max() - 40}, 70, 20, "Quit", [](Address, Address pw)
			{reference_to<My_window>(pw).quit(); } },
	checker_c1{ Point{100,100},100, 100, Menu::vertical, "col 1"},
	checker_c2{ Point{ 200,100 },100, 100, Menu::vertical, "col 2" },
	checker_c3{ Point{ 300,100 },100, 100, Menu::vertical, "col 3" },
	checker_c4{ Point{ 400,100 },100, 100, Menu::vertical, "col 4" }
{
	init_rect();
	for (int i = 0; i < rect.size(); ++i)
	{
		attach(rect[i]);
	}
	
	checker_c1.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(1, Color::blue); } });
	checker_c1.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(2, Color::red); } });
	checker_c1.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(3, Color::black); } });
	checker_c1.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(4, Color::yellow); } });

	checker_c2.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(5, Color::green); } });
	checker_c2.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(6, Color::white); } });
	checker_c2.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(7, Color::dark_yellow); } });
	checker_c2.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(8, Color::magenta); } });

	checker_c3.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(9, Color::cyan); } });
	checker_c3.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(10, Color::black); } });
	checker_c3.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(11, Color::red); } });
	checker_c3.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(12, Color::dark_blue); } });

	checker_c4.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(13, Color::dark_cyan); } });
	checker_c4.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(14, Color::dark_red); } });
	checker_c4.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(15, Color::dark_magenta); } });
	checker_c4.attach(new Button{ Point{ 0,0 },0,0, "1", [](Address, Address pw) { reference_to<My_window>(pw).change_color(16, Color::dark_green); } });

	attach(checker_c1);
	attach(checker_c2);
	attach(checker_c3);
	attach(checker_c4);
	
	attach(next_b);
	attach(quit_b);
}

class My_window2 :public Window
{
public:
	My_window2(Point xy, int w, int h, const string& s);

private:

	Point button_point;
	Button move_b;
	Button quit_b;
	static void cb_move(Address, Address pw) { reference_to<My_window2>(pw).move(); }
	Image im;
	void move();
	void quit2() { hide(); }
	Point im_point{ int(x_max() / 2), int(y_max() / 2) };


};

void My_window2::move()
{
	int xmax = x_max();
	int xmin = 1;
	int ymax = y_max();
	int ymin = 1;
	int dx = rand_int(-100, 100);
	int dy = rand_int(-100, 100);
	Point b = Point{ int(x_max() / 2), int(y_max() / 2) };
	
	move_b.move(dx, dy);
	im.move(dx, dy);
	
	redraw();	
}

My_window2::My_window2(Point xy, int w, int h, const string& s)
	: button_point{ int(x_max() / 2), int(y_max() / 2) },
	Window{ xy, w, h, s },
	move_b{button_point, 100, 100, "mover",cb_move },
	quit_b{ Point{ x_max() - 100, y_max() - 40 }, 70, 20, "Quit", [](Address, Address pw)
			{reference_to<My_window2>(pw).quit2(); } },
	im{ button_point, "Bishop.jpg" }
{		
	attach(move_b);
	attach(quit_b);
	attach(im);	
}

inline int rand_int(int min, int max)
{
	static default_random_engine ran;
	return uniform_int_distribution<>{min, max}(ran);
}

class Shape_window : public Window
{
public:
	Shape_window(Point xy, int w, int h, const string& s);
		
private:
	Button open_shapes;
	Menu shapes;
	Button quit_button;

	Button enter_circle;
	Button enter_square;
	Button enter_triangle;
	Button enter_hexagon;

	In_box xpoint{ Point{100, y_max() - 70}, 70, 20, "x: " };
	In_box ypoint{ Point{ 100, y_max() - 40 }, 70, 20, "y: " };


	In_box circle_radius{ Point{ 250, y_max() - 40 }, 70, 20, "Radius" };
	In_box side_length{ Point{ 250, y_max() - 40 }, 70, 20, "Side Length" };

	void draw_circle();
	void draw_square();
	void draw_triangle() { ; }
	void draw_hexagon();


	void hide_all();

	void opens();

	void circle();
	void square();
	void hexagon();
	void triangle();
	
	void quit() { hide(); }

	static void cb_shapes(Address, Address pw) { reference_to<Shape_window>(pw).opens(); }
	
	static void cb_circle(Address, Address pw) { reference_to<Shape_window>(pw).circle(); }
	static void cb_square(Address, Address pw) { reference_to<Shape_window>(pw).square(); }
	static void cb_hexagon(Address, Address pw) { reference_to<Shape_window>(pw).hexagon(); }
	static void cb_triangle(Address, Address pw) { reference_to<Shape_window>(pw).triangle(); }

	Point p_circle;
	int rad;
	Circle circ{ p_circle, rad };

	Point p_square{100, 100};
	int square_length = 100;
	Graph_lib::Rectangle sqr{ p_square, square_length, square_length };

	Point p_octagon;
	int octagon_length;
	Octagon oct{ p_octagon, octagon_length};


};

void Shape_window::circle()
{
	hide_all();
	circle_radius.show();
	enter_circle.show();

}
void Shape_window::square()
{
	hide_all();
	side_length.show();
	enter_square.show();
}
void Shape_window::hexagon()
{
	hide_all();
	side_length.show();
	enter_hexagon.show();
}
void Shape_window::triangle()
{
	hide_all();
	side_length.show();
	enter_triangle.show();
}

void Shape_window::hide_all()
{
	circle_radius.hide();
	enter_circle.hide();
	enter_hexagon.hide();
	enter_square.hide();
	enter_triangle.hide();
	side_length.hide();
}

Shape_window::Shape_window(Point xy, int w, int h, const string& s)
	:Window{ xy,w,h,s },
	open_shapes{ Point{x_max() - 120,20},90, 20, "Open Shapes",cb_shapes },
	shapes{ Point{x_max() - 120, 20}, 90, 20, Menu::vertical, "shapes"},
	quit_button{ Point{x_max() - 100, y_max() - 40}, 70, 20, "Quit", [](Address, Address pw) {reference_to<Shape_window>(pw).quit(); } },
	enter_circle{ Point{ 350, y_max() - 40 },50, 20, "Enter",[](Address, Address pw) {reference_to<Shape_window>(pw).draw_circle(); } },
    enter_square{ Point{ 350, y_max() - 40 },50, 20, "Enter",[](Address, Address pw) {reference_to<Shape_window>(pw).draw_square(); } },
    enter_triangle{ Point{ 350, y_max() - 40 },50, 20, "Enter",[](Address, Address pw) {reference_to<Shape_window>(pw).draw_triangle(); } },
    enter_hexagon{ Point{ 350, y_max() - 40 },50, 20, "Enter",[](Address, Address pw) {reference_to<Shape_window>(pw).draw_hexagon(); } }

{
	shapes.attach(new Button{ Point{0,0}, 0,0, "Circle", cb_circle } );
	shapes.attach(new Button{ Point{ 0,0 }, 0,0, "Square", cb_square });
	shapes.attach(new Button{ Point{ 0,0 }, 0,0, "Octagon", cb_hexagon });
	shapes.attach(new Button{ Point{ 0,0 }, 0,0, "eq-Triangle", cb_triangle });

	attach(shapes);
	shapes.hide();

	attach(open_shapes);

	attach(quit_button);

	attach(xpoint);
	attach(ypoint);

	attach(side_length);

	attach(circle_radius);
	
	attach(enter_circle);	
	attach(enter_square);
	attach(enter_hexagon);
	attach(enter_triangle);

	circ.set_color(Color::black);
	attach(circ);

	
	hide_all();
}

void Shape_window::opens()
{
	open_shapes.hide();
	shapes.show();
}

void Shape_window::draw_circle()
{
	rad = circle_radius.get_int();
	p_circle.x = xpoint.get_int();
	p_circle.y = ypoint.get_int();
	circ.set_radius(rad);
	circ.set_mid(0, Point{ p_circle.x - rad, p_circle.y - rad });
	circ.set_color(Color::black);
	redraw();

}

void Shape_window::draw_square()
{
	square_length = side_length.get_int();
	p_square.x = xpoint.get_int();
	p_square.y = ypoint.get_int();
	sqr.set_corner(0, Point{ p_square.x, p_square.y });
	sqr.set_width(square_length);
	sqr.set_height(square_length);
	sqr.set_color(Color::black);
	attach(sqr);
	redraw();

}
void Shape_window::draw_hexagon()
{
	octagon_length = side_length.get_int();
	p_octagon.x = xpoint.get_int();
	p_octagon.y = ypoint.get_int();
	oct.set_centre(Point{ p_octagon.x, p_octagon.y });
	oct.set_side(octagon_length);
	oct.set_color(Color::black);
	attach(oct);
	redraw();

}

