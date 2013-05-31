clear

Names = {'Hips', 'LHipJoint', 'LeftUpLeg', 'LeftLeg', 'LeftFoot', ...
               'RHipJoint', 'RightUpLeg','RightLeg','RightFoot',...
               'LowerBack', 'Spine', 'Spine1', 'Neck', 'Neck1', 'Head',...
               'LeftShoulder', 'LeftArm', 'LeftForeArm', 'LeftHand',...
               'RightShoulder','RightArm','RightForeArm','RightHand'};
Indices= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

name2ind = containers.Map(Names, Indices);
ind2name = containers.Map(Indices, Names);

%parent= {-1, 0, 1, 2, 3, 0, 5, 6, 7, 0, 9, 10, 11, 12, 13, 11, 15, 16, 17, 11, 19, 20, 21};

parent={0,1,2,3,4,1,6,7,8,1,10,11,12,13,14,12,16,17,18,12,20,21,22};

raw_data = dlmread('data');

[nJoints, n] = size(raw_data);
nFrame = n / 16;
flattened_data = reshape(raw_data, nJoints, 16, nFrame);

clf;
figure(1);
plotRootTraj(flattened_data(1,[4,8,12], :));
axis equal;
%{
for i=5:5%nFrame
  draw_skel(parent, flattened_data(:,:,i));
end
%}