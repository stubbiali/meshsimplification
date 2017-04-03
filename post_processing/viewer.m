% Plot a mesh with or without distributed data points.
% The mesh is given through a .mat file comprising:
%   - nodes  three-columns matrix storing the grid nodes
%   - elems  three-columns matrix storing the id's of the vertices of each
%               triangle
%   - data   three-columns matrix storing the data (if any)
%
% \param filename   name of the .mat file storing the mesh
function viewer(filename)
    % Open the file and extract some info
    load(filename);
    numElems = size(elems);
    
    % Open the plot window
    figure;
    hold on;
    
    % Plot the elements
    for i = 1:numElems
        % Some shortcuts
        id1 = elems(i,1);
        id2 = elems(i,2);
        id3 = elems(i,3);
        ids = [id1 id2 id3 id1];
        
        % Plot
        fill3(nodes(ids,1), nodes(ids,2), nodes(ids,3), 'b')
    end
    
    % Possibly plot the data
    if (exist('data') > 0)
        plot3(data(:,1), data(:,2), data(:,3), 'r.', 'Markersize', 5)
    end
    
    % Figure settings
    xlabel('x')
    ylabel('y')
    zlabel('z')
    h = title(filename);
    set(h, 'interpreter', 'none');
    axis equal
    
    % Close dataset
    clear nodes elems data
