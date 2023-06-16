void cam_yan(int dyan)
{
	xcam_yan+=dyan;
	clip_angle(&xcam_yan);
}

void cam_xan(int dxan)
{
	xcam_xan+=dxan;
	clip_angle(&xcam_xan);
}

void cam_zan(int dzan)
{
	xcam_zan+=dzan;
	clip_angle(&xcam_zan);
}
