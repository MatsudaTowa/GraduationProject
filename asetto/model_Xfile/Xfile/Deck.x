xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 24;
 -12.94335;3.63320;-19.90291;,
 12.94335;3.63320;-19.90291;,
 12.94335;0.30645;-19.90291;,
 -12.94335;0.30645;-19.90291;,
 14.46399;3.63320;-18.72129;,
 14.46399;3.63320;18.72129;,
 14.46399;0.30645;18.72129;,
 14.46399;0.30645;-18.72129;,
 12.94335;3.63320;19.90291;,
 -12.94335;3.63320;19.90291;,
 -12.94335;0.30645;19.90291;,
 12.94335;0.30645;19.90291;,
 -14.46399;3.63320;18.72129;,
 -14.46399;3.63320;-18.72129;,
 -14.46399;0.30645;-18.72129;,
 -14.46399;0.30645;18.72129;,
 12.94335;3.63320;18.72129;,
 -12.94335;3.63320;18.72129;,
 -12.94335;3.63320;-18.72129;,
 12.94335;3.63320;-18.72129;,
 -12.94335;0.30645;18.72129;,
 12.94335;0.30645;18.72129;,
 -12.94335;0.30645;-18.72129;,
 12.94335;0.30645;-18.72129;;
 
 26;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;9,8,16,17;,
 4;12,17,18,13;,
 4;17,16,19,18;,
 4;16,5,4,19;,
 4;18,19,1,0;,
 4;20,21,11,10;,
 4;14,22,20,15;,
 4;22,23,21,20;,
 4;23,7,6,21;,
 4;3,2,23,22;,
 3;12,9,17;,
 4;12,15,10,9;,
 3;15,20,10;,
 3;6,11,21;,
 4;5,8,11,6;,
 3;5,16,8;,
 3;4,1,19;,
 4;4,7,2,1;,
 3;7,23,2;,
 3;3,22,14;,
 4;13,0,3,14;,
 3;0,13,18;;
 
 MeshMaterialList {
  3;
  26;
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  0,
  1,
  2,
  2,
  1,
  0,
  0,
  1,
  2,
  2,
  1,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Deskto\\image.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\450-2009081909414330861.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
}
