function [] = plotRootTraj(pos)
X = pos(1,:,1);
Y = pos(1,:,2);
Z = pos(1,:,3);
plot3(X(:), Y(:), Z(:));
%a = 1;
end