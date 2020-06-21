#include "TextWriter.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_truetype.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include "stb_image.h"


TextWriter* TextWriter::instance = 0;
GLuint texId;
stbtt_bakedchar cdata[96];

// Singleton
TextWriter* TextWriter::getInstance() {
	if (instance == 0) {
		instance = new TextWriter();
	}

	instance->initWriter();

	return instance;
}

TextWriter::TextWriter() {}

TextWriter::~TextWriter() {}

void TextWriter::writeText(glm::vec3 loc, std::string text) {
    float x = loc.x;
    float y = loc.y;
    float z = loc.z;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texId);

    stbtt_aligned_quad q;
    tigl::begin(GL_QUADS);

    for (int i = 0; i < text.size(); i++) {
        stbi_set_flip_vertically_on_load(true);
        if (text[i] >= 32 && text[i] < 128) {
            stbtt_GetBakedQuad(cdata, 512, 512, text[i] - 32, &x, &y, &q, 1);
            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x0, -q.y0, z), glm::vec2(q.s0, q.t0)));
            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x1, -q.y0, z), glm::vec2(q.s1, q.t0)));
            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x1, -q.y1, z), glm::vec2(q.s1, q.t1)));
            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x0, -q.y1, z), glm::vec2(q.s0, q.t1)));
        }
    }
    tigl::end();
}

void TextWriter::initWriter() {
    unsigned char* ttf_buffer = new unsigned char[1 << 20];
    unsigned char* temp_bitmap = new unsigned char[512 * 512];
    fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer, 0, 64.0, temp_bitmap, 512, 512, 32, 96, cdata);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] ttf_buffer;
    delete[] temp_bitmap;
}

