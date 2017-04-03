clc
clear variables
clear variables -global
close all

dataset = 'dataset/brain.mat';
load(dataset);

dataset = 'dataset/elems_to_plot.mat';
load(dataset);

%{
elems_to_plot = [52360 52389 53123 53121 53185 81878 81856 81791 81792 52394 ...
    52395 52396 52392 52390 52389 52357 52358 52359 52362 52363 53124 ...
    53186 53187 52364 52367 52361 52387 52388 52393 52397 52398 52399 ...
    53054 53055 53056 53122 52368 52422 53133 52386 52366 81790 53053 ...
    81788 81787 81785 81784 52365 52421 52423 52426 53130 53125 53048 ...
    53049 53047 53045 53042 53052 53051 53050 53131 53128 53129 81784];
%}

elems_to_plot = [49500:53500 80500:82000];

figure;
hold on
for i = 1:length(elems_to_plot)
    a = nodes(elems(elems_to_plot(i),1),:);
    b = nodes(elems(elems_to_plot(i),2),:);
    c = nodes(elems(elems_to_plot(i),3),:);
    
    x = [a(1) b(1) c(1) a(1)];
    y = [a(2) b(2) c(2) a(2)];
    z = [a(3) b(3) c(3) a(3)];
    
    plot3(x, y, z, 'r');
    fill3(x, y, z, 'w');
end
view([51 -12])