int main()
{
	Lines_window w{ Point{ 100,100 },600,400,"lines" };
	My_window win{ Point{500,100}, 700, 700, "My Window" };
	My_window2 win2{ Point{300,200 }, 400, 400, "My Window 2" };
	Shape_window win3{ Point{ 400,100 }, 600, 600, "My Window 3" };
	return gui_main();
}
