#include <iostream>
#include <fstream>
#include <string>
#include "http/http.h"
#include <cstdio>
using namespace httplib;
using namespace std;

const char *html = R"(
<form id="formElem">
  choose an image
  <input type="file" name="image_file" accept="image/*">
  choose a txt
  <input type="file" name="text_file" accept="text/*"></br>
  <input type="submit">
</form>
<script>
  formElem.onsubmit = async (e) => {
    e.preventDefault();
    let res = await fetch('/post', {
      method: 'POST',
      body: new FormData(formElem)
    });
    console.log(await res.text());
  };
</script>
)";

std::string log(const Request &req, const Response &res);

int main(void) {

    Server svr;

    //file download
    svr.Get("/hi", [](const Request & /*req*/, Response &res) {
        string image_url = "https://test-1301539759.cos.ap-guangzhou.myqcloud.com/public/huaping/993VA0104_20200725155144_10.jpg";

        Client cli(const_cast<char*>(image_url.c_str()));
        auto pos = find(image_url.rbegin(), image_url.rend(), '/');
        string img_name = image_url.substr(distance(pos, image_url.rend()));
        cout<<img_name<<endl;
        string save_dir = "../save/";

        cli.downloadAs(image_url,save_dir+img_name);

        res.set_content("Hello World!121212121", "text/plain");
    });

    svr.Post("/hp", [](const Request &req, Response &res) {
        auto image_file = req.get_file_value("img_url");
//        std::cout << image_file.content.length() << std::endl;
//        std::cout << image_file.name << std::endl;
//        std::cout << image_file.content << std::endl;
        res.set_content(image_file.content, "text/plain");
    });

    // file upload
    svr.Get("/upload", [](const Request & /*req*/, Response &res) {
        res.set_content(html, "text/html");
    });
    svr.Post("/post", [](const Request &req, Response &res) {
        auto image_file = req.get_file_value("image_file");
        auto text_file = req.get_file_value("text_file");

        cout << "image file length: " << image_file.content.length() << endl
             << "image file name: " << image_file.filename << endl
             << "text file length: " << text_file.content.length() << endl
             << "text file name: " << text_file.filename << endl;
        string save_dir = "../save/";
        {
            ofstream ofs(save_dir+image_file.filename, ios::binary);
            ofs << image_file.content;
        }
        {
            ofstream ofs(save_dir+text_file.filename);
            ofs << text_file.content;
        }
        res.set_content("done", "text/plain");
    });

    // log
    svr.set_logger([](const Request &req, const Response &res) {
        printf("%s", log(req, res).c_str());
    });

    svr.listen("0.0.0.0", 8080);
}


std::string log(const Request &req, const Response &res) {
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";
    snprintf(buf, sizeof(buf), "%s %s %s\n", req.method.c_str(),
             req.version.c_str(), req.path.c_str());

    s += buf;
    snprintf(buf, sizeof(buf), "%s %s %s", req.remote_addr.c_str(),
             to_string(req.remote_port).c_str(), req.get_header_value("Content-Type").c_str());

    s += buf;

    return s;
}

//accept限制图像格式，accept="image/*"不限制图片格式，
//只要是图片都可以上传。如果写成accept="image/gif"的话，那就是只能上传gif格式的图片>
// <!- accept="video/*" ->