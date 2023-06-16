#define move 10
#define zmove 10000
#define dif 0

int check_key(void)
{
	key=getch();
	if (!key) key=getch();
	switch (key)
	{
		case debug_frame:
			if (space) space=0;
			else space=1;
			break;
		case cam_yan_up:
			camera.yan+=;
					camera.x+=move;
					clip_angle(&camera.yan);
					pressed=1;
					break;
				case 75:
					//camera.yan-=1;
					camera.x-=move;
					clip_angle(&camera.yan);
					pressed=1;
					break;
				case 80:
					//camera.xan+=1;
					camera.y+=move;
					clip_angle(&camera.xan);
					pressed=1;
					break;
				case 72:
					//camera.xan-=1;
					//clip_angle(&camera.xan);
					camera.y-=move;
                                        pressed=1;
					break;
				case 'a':
					if (dif)
					{
						camera.zan+=1;
						clip_angle(&camera.zan);
					}
					else camera.z+=zmove;
					break;
				case 'z':
					if (dif)
					{
						camera.zan-=1;
						clip_angle(&camera.zan);
					}
					else camera.z-=zmove;
					break;					break;
				}
			}
			else
			switch (key)
			{
				case 13:
					if (space) space=0;
					else space=1;
					break;
				case 27:
					timetogo=1;
					break;
			}
