% Driver for plotting a triangular surface mesh, either with or without
% distributed data. The mesh must be stored in a .mat file.
% Grids currently provided:
%   - dataset/brain
%   - dataset/brain_10000_033_033_033
%   - dataset/bunny
%   - dataset/bunny_5000_033_033_033
%   - dataset/pawn
%   - dataset/pawn_1000_033_033_033  
%   - dataset/pawn_500_033_033_033

clc
close all
beep off

filename = 'dataset/brain';
viewer(filename);