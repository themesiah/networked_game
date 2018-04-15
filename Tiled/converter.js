'use strict';

const fs = require('fs');

function delete_unused_objects(map_json) {
	delete map_json.infinite;
	delete map_json.nextobjectid;
	delete map_json.orientation;
	delete map_json.renderorder;
	delete map_json.tiledversion;
	delete map_json.type;
	delete map_json.version;

	for (var i = 0; i < map_json.layers.length; ++i) {
		var layer = map_json.layers[i];
		delete layer.opacity;
		delete layer.type;
		delete layer.visible;
		delete layer.x;
		delete layer.y;
		delete layer.height;
		delete layer.width;
		layer.zindex = layer.properties.zindex;
		layer.tileset = layer.properties.tileset;
		delete layer.properties;
		delete layer.propertytypes;
	}
}

function get_corresponding_tileset_size(tilesets, num) {
	for (var z = 0; z < tilesets.length; ++z) {
		var tileset = tilesets[z];
		if (num < tileset.firstgid) {
			return z-1;
		}
	}
	return tilesets.length-1;
}

function normalize_tiles(map_json) {
	for (var i = 0; i < map_json.layers.length; ++i) {
		var layer = map_json.layers[i];
		var data = layer.data;
		var tileset_size = 0;
		for (var j = 0; j < data.length; ++j) {
			if (data[j] != 0) {
				if (tileset_size == 0) {
					var tileset_index = get_corresponding_tileset_size(map_json.tilesets, data[j]);
					tileset_size = map_json.tilesets[tileset_index].firstgid;
				}
				data[j] = (data[j]-tileset_size) + 1;
			}
		}
		tileset_size = 0;
	}
	delete_unused_objects2(map_json);
}

function delete_unused_objects2(map_json) {
	delete map_json.tilesets;
	
}

var path = process.argv[2];
var out_path = path;
out_path = out_path.substring(0, out_path.lastIndexOf('.')) + "_out.json";
var partial_path = path.substring(0, path.lastIndexOf("/"));
var data = fs.readFileSync(path);
var map_json = JSON.parse(data);

delete_unused_objects(map_json);

var tilesets = map_json.tilesets;
normalize_tiles(map_json);
delete_unused_objects2(map_json);
fs.writeFileSync(out_path, JSON.stringify(map_json));



