void init_obj_sys(int num)
{
	view_dist=(a_cos(30)*160)>>8;
	objects=(tobject*)calloc(num,sizeof(tobject));
	if (objects==NULL) {
		printf("Mem Error:Objects calloc\n");
		exit(0);
	}
	numobjects=num;
}

void kill_obj_sys(void)
{
	free(objects);
}

void init_object(int n,int tsides,int tvertices,int* sdata_area,int* data_area,int tx,int ty,int tz)
{
	objects[n].visible=1;
	objects[n].x=tx;
	objects[n].y=ty;
	objects[n].z=tz;
	objects[n].xan=0;
	objects[n].yan=0;
	objects[n].zan=0;
	objects[n].sdata=sdata_area;
	objects[n].data=data_area;
	objects[n].vertices=tvertices;
	objects[n].sides=tsides;
	objects[n].temp=(int*)calloc((tvertices*3),sizeof(int));
	if (objects[n].temp==NULL) {
		printf("Mem Error:Temp calloc\n");
		exit(0);
	}
}

void set_cam_xyz(long tx, long ty, long tz)
{
	camera.x=tx;
	camera.y=ty;
	camera.z=tz;
}

void put_object(int c)
{
	int i,j,z,t,k;

	for (i=0;i<objects[c].sides;i++)
	{
		points=&(objects[c].temp[0]);
		vertices=objects[c].vertices;

		t=tentimes[i];
		indexes[nextpoly]=i;
		z=0;
		for (j=0;j<4;j++) {
			k=3*objects[c].sdata[t+j];
			z+=objects[c].temp[k+2];
		}
		averages[nextpoly]=(z>>2);
		idobjs[nextpoly]=c;
		nextpoly++;
	}
}

void twist_object(int c,int dx,int dy,int dz)
{
	long dist_from_cam;
	int i,j,k,t;

	objects[c].visible=1;
	objects[c].xan+=dx;
	objects[c].yan+=dy;
	objects[c].zan+=dz;

	//dist_from_cam=distance(objects[c].x,objects[c].y,objects[c].z);
	if (objects[c].visible)
	{
		gxan=objects[c].xan;
		gyan=objects[c].yan;
		gzan=objects[c].zan;
		set_matrix();
		set_eye_matrix();

		for (i=0;i<objects[c].vertices;i++)
		{
			k=threetimes[i];
			lx=objects[c].data[k];
			ly=objects[c].data[k+1];
			lz=objects[c].data[k+2];
			rotate_pt();
			obj2world(c);
			rotate_eye_pt();
			world2cam(c);
			depth_emul();
			screen_center();
			objects[c].temp[k]=gx;
			objects[c].temp[k+1]=gy;
			objects[c].temp[k+2]=gz;
		}

	}
}

void do_update(void)
{
	int i,j,thenum,z,temp,t,n,k,tmp,tmp1,found;
	int nx,ny,nz;
	int x,y;
	int p;
	int gotpoint=0;
	int *polygons;

	//now we sort only the visible sides :)


	curious=nextpoly;

	//now we can draw the sorted visible polygons...
	for (i=0;i<nextpoly;i++)
	{
		//set our global points to temp
		points=(int*)&(objects[idobjs[i]].temp[0]);
		thenum=tentimes[indexes[i]];
		poly_color=(indexes[i] << 3) + 64;
		maxy=-15000;
		maxx=-15000;
		miny=15000;
		minx=15000;
		gotpoint=0;
		for (j=0;j<4;j++)
		{
			x=r_points[];
			y=r_points[];


			if (on_screen(&x,&y)) gotpoint=1;
			if (fy>maxy) {
				maxy=y;
				maxyindex=j;
			}
			if (x>maxx) {
				maxx=x;
				maxxindex=j;
			}
			if (x<minx) {
				minx=x;
				minxindex=j;
			}
			if (y<miny) {
				miny=y;
				minyindex=j;
			}
		}
		//set_lambert();
		//gouraud_poly();
		/* if no points were on the screen check for */
		/* polys that may cover a portion of the screen */
		if (!gotpoint)
		{
			if (minx>=x_right_clip) goto skip_poly;
			if (maxx<=x_left_clip) goto skip_poly;
			if (miny>=y_top_clip) goto skip_poly;
			if (maxy<=y_bottom_clip) goto skip_poly;
		}
		fill_poly();
		polys_drawn++;
skip_poly:
	}
}

