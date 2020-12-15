#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofDisableArbTex();
	ofSetLineWidth(2);

	this->cap_size = cv::Size(640, 360);
	this->rect_size = 20;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_image_list.push_back(move(image));
			this->cv_rect_list.push_back(rect);
			this->rect_frame_list.push_back(frame);
			this->draw_rect_size_list.push_back(this->rect_size);
		}
	}

	auto file_path = "D:\\MP4\\video2.mp4";
	this->cap.open(file_path);
	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	
	this->cam.begin();
	ofTranslate(this->cap_size.width * -0.5 + this->rect_size * 0.5, this->cap_size.height * 0.5 - this->rect_size * 0.5);
	ofRotateX(180);
	ofRotateY(45);

	for (int z = 0; z < 300; z += 60) {

		for (int i = 0; i < this->rect_frame_list.size(); i++) {

			ofPushMatrix();
			ofTranslate(this->cv_rect_list[i].x, this->cv_rect_list[i].y, -z + 150 - 1);
			ofRotateX(180);

			auto noise_value = ofNoise(this->cv_rect_list[i].x * 0.005, this->cv_rect_list[i].y * 0.005, -z + 150 - 1, ofGetFrameNum() * 0.005);

			if (noise_value > 0.52) {

				int frame_index = (int)(ofGetFrameNum() * 0.42 + z * 0.5) % this->number_of_frames;

				cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rect_list[i]);
				tmp_box_image.copyTo(this->rect_frame_list[i]);

				this->rect_image_list[i]->update();
				this->rect_image_list[i]->getTexture().bind();

				ofSetColor(255);
				ofFill();
				ofDrawPlane(glm::vec3(), this->rect_size, this->rect_size);
				this->rect_image_list[i]->unbind();

				ofSetColor(39);
				ofNoFill();
				ofDrawRectangle(glm::vec3(this->rect_size * -0.5, this->rect_size * -0.5, 0), this->rect_size, this->rect_size);
			}

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}