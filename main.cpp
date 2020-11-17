//
// Created by zj on 2020/11/16.
//
#include <iostream>
#include <string>
#include "http/httplib.h"
#include "downloadModule/HttpClient.h"
#include "CJSON/CJsonObject.hpp"
#include <pthread.h>
#include <mutex>

#include <cstdio>
using namespace httplib;
using namespace std;


//const char *html = R"(
//<form id="formElem">
//  choose an image
//  <input type="file" name="image_file" accept="image/*">
//  choose a txt
//  <input type="file" name="text_file" accept="text/*"></br>
//  <input type="submit">
//</form>
//<script>
//  formElem.onsubmit = async (e) => {
//    e.preventDefault();
//    let res = await fetch('/post', {
//      method: 'POST',
//      body: new FormData(formElem)
//    });
//    console.log(await res.text());
//  };
//</script>
//)";


std::string log(const Request &req, const Response &res);
struct threadParam {
    int threadID;
    pair<string, string> imageURLPair;
};

static std::mutex mutx;
static int threadCount,downloadCount;

// Output conversion
static map<int,int> classfies{{0,1},{1,2},{2,4}}; // rule 1,2,4 for checking failure in downloading image


static void *downloadImage(void *para){
//    convert in force
    threadParam *p = (threadParam *)para;
    int tid = p->threadID;
//    cout << "thread id :" << tid <<" "<< endl;
    HttpClient client;
    bool downloadFlag = false;
    downloadFlag = client.downloadAs(p->imageURLPair.first, p->imageURLPair.second);
    if(downloadFlag){
        cout << p->imageURLPair.second << endl;
        mutx.lock();
        downloadCount += classfies[tid];
        threadCount ++;
        mutx.unlock();
    } else{
        mutx.lock();
        threadCount ++;
        mutx.unlock();
    }
    pthread_exit(NULL);
}

#define NUM_THREADS 3

int main(void) {
    Server svr;

    static int num = 0;
    static int flag = 0;

    svr.Post("/download", [](const Request &req, Response &res){
        cout << endl << endl << "num: " << ++num << endl;
        if(flag == 0)
            flag = 1;
        else
            flag = 0;
        vector<pair<string, string>> imageURLPair;
        for(int i = 0; i < 3; i++){
            string temp = "img_url" + to_string(i + 1);
            const char *key = temp.c_str();
            auto formdataURL = req.get_file_value(key);
            string imageURL = formdataURL.content;
            auto pos = find(imageURL.rbegin(), imageURL.rend(), '/');
            string imageName = to_string(flag) + imageURL.substr(distance(pos, imageURL.rend()));
            string saveDir = "../save/" + imageName;
            imageURLPair.push_back(make_pair(imageURL, saveDir));
        }
        for(int i=0; i<3;i++){
//            cout<<imageURLPair[i].first<<" " <<imageURLPair[i].second<<endl;
        }

        //file download
        pthread_t threads[NUM_THREADS];
        int indexes[NUM_THREADS];// 用数组来保存i的值
        int threadFlag;

        downloadCount = 0;
        threadCount = 0;
        for( int i = 0; i < NUM_THREADS; i++ ){
            threadParam *param = new threadParam();
            param->threadID = i;
            param->imageURLPair = imageURLPair[i];
            indexes[i] = i;
            threadFlag = pthread_create(&threads[i], NULL, &downloadImage, param);
            if (threadFlag){
                cout << "Error in creating thread" << threadFlag << endl;
                return;
            }
        }
        //wait for download threads
        while(threadCount != 3){
        }
        cout << "downloadCount: "<<downloadCount <<endl;

        neb::CJsonObject oJson("");
        oJson.Add("msg", "downloaded!");
        oJson.Add("code", 1);

        res.set_content(oJson.ToString(), "application/json");
    });

    // file upload
//    svr.Get("/upload", [](const Request & /*req*/, Response &res) {
//        res.set_content(html, "text/html");
//    });
//    svr.Post("/post", [](const Request &req, Response &res) {
//        auto image_file = req.get_file_value("image_file");
//        auto text_file = req.get_file_value("text_file");
//
//        cout << "image file length: " << image_file.content.length() << endl
//             << "image file name: " << image_file.filename << endl
//             << "text file length: " << text_file.content.length() << endl
//             << "text file name: " << text_file.filename << endl;
//        string save_dir = "../save/";
//        {
//            ofstream ofs(save_dir+image_file.filename, ios::binary);
//            ofs << image_file.content;
//        }
//        {
//            ofstream ofs(save_dir+text_file.filename);
//            ofs << text_file.content;
//        }
//        res.set_content("done", "text/plain");
//    });

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

