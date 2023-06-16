void make_sphere()
{
	pt = &points[0];
	pt2 = &r_points[0];

	randomize();

	for (i = 0; i < vertices; i++)
	{
		points[i].x = size;
		points[i].y = size;
		points[i].z = size;
		xan = random(256);
		yan = random(256);
		zan = random(256);
		//yan+=4;
		//if (yan == 0) xan++;
		//if (xan == 252) yan+=4;
		//if (yan == 252) zan+=4;
		set_matrix(&xan, &yan, &zan);
		rotate();
		points[i].x = r_points[i].x;
		points[i].y = r_points[i].y;
		points[i].z = r_points[i].z;
		//printf("%d,%d,%d\n",points[i].x, points[i].y, points[i].z);
		pt++;
		pt2++;
	}
}