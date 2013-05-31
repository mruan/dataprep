function [] = draw_skel(dict, pos)
% dict is a data structure that holds heirarchical information
% pos stores all joints transformations (4x4)

N = numel(dict);
% for every joint, draw a line connecting
% itself with its parent
hold on
for i= 1:N
  if dict{i} <= 0
    continue;
  end
  
%  fprintf('Joint %d\n', i);
  P  = pos([i, dict{i}], [4, 8, 12]);
%  Xc = pos(i, [4,8,12]);
%  Xp = pos(dict{i}, [4,8,12]);
  plot3(P(:,1), P(:,2), P(:,3));
end
hold off
end