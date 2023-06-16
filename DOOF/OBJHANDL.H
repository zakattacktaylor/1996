void set_cam_ang(int,int,int);
void set_cam_xyz(int,int,int);
void put_objects(void);
void calc_objects(void);

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
	int k;
	int t,s;
	float x1,y1,z1;
	float x,y,z;
	int xf,yf,zf;
	int p;

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

void set_cam_ang(tax,tay,taz)
{
	int x,y,z;

	x=camera.xan+tax;
	y=camera.yan+tay;
	z=camera.zan+taz;
	clip_angles(&x,&y,&z);
	camera.xan=x;
	camera.yan=y;
	camera.zan=z;
}

void set_cam_xyz(tx,ty,tz)
{
	camera.x=tx;
	camera.y=ty;
	camera.z=tz;
}

void put_object(int c)
{
	int i,j,thenum,z,temp,t,n,k,tmp,tmp1,found;
	int nx,ny,nz;
	int nextpoly=0;
	int x,y;
	int x1,y1,x2,y2,x3,y3;
	int p;

	if (!objects[c].visible) return;

	//printf("\n\n");
	for (i=0;i<objects[c].sides;i++)
	{
		t=tentimes[i];
		
		indexes[nextpoly]=i;
		z=0;
		for (j=0;j<4;j++) {
			k=3*objects[c].sdata[t+j];
			z+=objects[c].temp[k+2];
		}
		averages[nextpoly]=(z>>2);
		nextpoly++;
	}
	
	for (k=1;k<nextpoly;k++)
	{
		tmp=averages[k];
		tmp1=indexes[k];
		i=k-1;
		found=0;
		while ((i>=0) &&(!found)) {
			if (tmp < averages[i]) {
				averages[i+1]=averages[i];
				indexes[i+1]=indexes[i];
				i--;
			}
			else found=1;
		}
		averages[i+1]=tmp;
		indexes[i+1]=tmp1;
	}

	//set our global points to temp
	points=(int*)&(objects[c].temp[0]);
	
	curious=nextpoly;
	//now we can draw the sorted visible polygons...
	for (i=3;i<nextpoly;i++)                       
	{
		thenum=tentimes[indexes[i]];                
		//if (indexes[i]>31) poly_color=        
		poly_color=128;
		//poly_shades=32;
		//gxan=objects[c].sdata[thenum+7]+objects[c].xan;
		//gyan=objects[c].sdata[thenum+8]+objects[c].yan;
		//gzan=objects[c].sdata[thenum+9]+objects[c].zan;
		//clip_angles(&gxan,&gyan,&gzan);
		//illuminate();
		maxy=-5000;
		maxx=-5000;
		miny=5000;
		minx=5000;
		for (j=0;j<4;j++)
		{
			sides[j]=objects[c].sdata[thenum+j];
			k=threetimes[objects[c].sdata[thenum+j]];
			y=objects[c].temp[k+1];
			x=objects[c].temp[k];
			if (y>maxy) {
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
		//if ((maxy>0)&&(miny<screen_height))
		//set_lambert();
		fill_poly();
	}
	//printf("PUT_OBJECT\n");
}

void twist_object(int c,int dx,int dy,int dz)
{
	long dist_from_cam;
	int xan,yan,zan,x,y,z;
	int i,j,k,t;

	objects[c].visible=1;
	xan=objects[c].xan+dx;
	yan=objects[c].yan+dy;
	zan=objects[c].zan+dz;
	clip_angles(&xan,&yan,&zan);
	objects[c].xan=xan;
	objects[c].yan=yan;
	objects[c].zan=zan;

	dist_from_cam=distance(camera.x,camera.y,camera.z,objects[c].x,objects[c].y,objects[c].z);
	if (objects[c].visible) 
	{
		gxan=objects[c].xan;
		gyan=objects[c].yan;
		gzan=objects[c].zan;
		//clip_angles(&gxan,&gyan,&gzan);                
		set_matrix();
		
		for (i=0;i<objects[c].vertices;i++) 
		{
			k=threetimes[i];
			lx=objects[c].data[k];
			ly=objects[c].data[k+1];
			lz=objects[c].data[k+2];
			rotate_pt();
			gx=lx;
			gy=ly;
			gz=dist_from_cam-lz;
			//if (gz<0) {
			//        objects[c].visible=0;
			//        return;
			//}
			depth_emul();
			objects[c].temp[k]=gx+160+objects[c].x;
			objects[c].temp[k+1]=gy+120+objects[c].y;
			objects[c].temp[k+2]=lz;
		}

	}
	//printf("TWIST_OBJECT\n");
}

