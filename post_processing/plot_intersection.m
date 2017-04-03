clc
clear variables
clear variables -global
close all

tr1_x = [15.5044 10.5258 12.0973 15.5044];
tr1_y = [-66.2794 -68.3813 -64.1927 -66.2794];
tr1_z = [25.4752 28.902 27.1964 25.4752];

tr2_x = [11.6113 10.8817 12.1078 11.6113];
tr2_y = [-66.3729 -63.6243 -66.6808 -66.3729];
tr2_z = [24.7187 27.9624 27.795 24.7187];

figure
plot3(tr1_x, tr1_y, tr1_z, 'b')
hold on
plot3(tr2_x, tr2_y, tr2_z, 'r')