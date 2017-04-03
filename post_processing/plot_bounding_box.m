clc
clear variables 
clear variables -global
close all

load('dataset/pawn.mat');

nodes = [nodes(:,1) nodes(:,3) nodes(:,2)];

figure(1);
hold off
for i = 1:length(elems)
    vx = [nodes(elems(i,:),1); nodes(elems(i,1),1)];
    vy = [nodes(elems(i,:),2); nodes(elems(i,1),2)];
    vz = [nodes(elems(i,:),3); nodes(elems(i,1),3)];
    plot3(vx, vy, vz, 'k');
    hold on
    fill3(vx, vy, vz, [0.5 0.5 0.5])
end
axis equal
axis off

ne = [-Inf -Inf -Inf];
sw = -ne;
for i = 1:length(nodes)
    if nodes(i,1) > ne(1)
        ne(1) = nodes(i,1);
    end
    if nodes(i,1) < sw(1)
        sw(1) = nodes(i,1);
    end
    
    if nodes(i,2) > ne(2)
        ne(2) = nodes(i,2);
    end
    if nodes(i,2) < sw(2)
        sw(2) = nodes(i,2);
    end
    
    if nodes(i,3) > ne(3)
        ne(3) = nodes(i,3);
    end
    if nodes(i,3) < sw(3)
        sw(3) = nodes(i,3);
    end
end

dl = [-Inf -Inf -Inf];
for i = 1:length(elems)
    A = nodes(elems(i,1),:);
    B = nodes(elems(i,2),:);
    C = nodes(elems(i,3),:);
    
    for j = 1:3
        if (abs(A(j)-B(j)) > dl(j))
            dl(j) = abs(A(j)-B(j));
        end
        if (abs(B(j)-C(j)) > dl(j))
            dl(j) = abs(B(j)-C(j));
        end
        if (abs(C(j)-A(j)) > dl(j))
            dl(j) = abs(C(j)-A(j));
        end
    end
end

N = round((ne-sw)./dl);
dl = (ne-sw)./N;

for i = 0:N(1)
    for j = 0:N(2)
        plot3([sw(1)+i*dl(1) sw(1)+i*dl(1)], [sw(2)+j*dl(2) sw(2)+j*dl(2)], ...
            [sw(3) ne(3)], 'b');
    end
end

for i = 0:N(1)
    for j = 0:N(3)
        plot3([sw(1)+i*dl(1) sw(1)+i*dl(1)], [sw(2) ne(2)], ...
            [sw(3)+j*dl(3) sw(3)+j*dl(3)], 'b')
    end
end

for i = 0:N(2)
    for j = 0:N(3)
        plot3([sw(1) ne(1)], [sw(2)+i*dl(2) sw(2)+i*dl(2)], ...
            [sw(3)+j*dl(3) sw(3)+j*dl(3)], 'b')
    end
end

%xlabel('x')
%ylabel('y')
%zlabel('z')
    