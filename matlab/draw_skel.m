function [] = draw_skel(dict, pos)
% dict is a data structure that holds heirarchical information
% pos stores all joints transformations (4x4)

[nJoint, nFrame, ~] = size(pos);

% for every joint, draw a line connecting
% itself with its parent
hold on
for j= 5:5
    for i= 1:nJoint
      if dict{i} <= 0
        continue;
      end

    %  fprintf('Joint %d\n', i);
      P = pos([i, dict{i}], j, [4, 8, 12]);
      p = reshape(P, 2, 3); 
    %  Xc = pos(i, [4,8,12]);
    %  Xp = pos(dict{i}, [4,8,12]);
      plot3(p(:,1), p(:,2), p(:,3));
    end

end
hold off
end