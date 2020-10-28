# coding=utf-8 
import requests
import csv
import _thread
import time


def print_time( threadName, delay):
   count = 0
   while count < 5:
      time.sleep(delay)
      count += 1
      print ("%s: %s" % ( threadName, time.ctime(time.time()) ))

def test(threadName, delay):
    while(1):
         count = 0


         url_post = 'http://0.0.0.0:8080/download'

         headers = {
               'server-name':'cam-check-svc',
               'apikey':'0a3243e5-5dd4-4f97-a05f-8e909ab515d4--veqXIEQyFwpvEgtodr32RZcTDTcMivFZ',
         }
    
         # data = {
         #    'img_url':'https://test-1301539759.cos.ap-guangzhou.myqcloud.com/public/negative/024VA0105_20200725190041_10.jpg'        
         # }
         data = {
            'img_url1':'https://test-1301539759.cos.ap-guangzhou.myqcloud.com/public/sample/image_00001.jpg',
            'img_url2':'https://test-1301539759.cos.ap-guangzhou.myqcloud.com/public/sample/image_00002.jpg',
            'img_url3':'https://test-1301539759.cos.ap-guangzhou.myqcloud.com/public/sample/image_00003.jpg'
         }
         response = requests.post(url_post, files=data, headers=headers)

         # print response.text
         count  = count +1
         print('------------------')
         print(count)
         print(response.content)
         print("response.status_code",response.status_code)
         time.sleep(0.1)





# 创建两个线程
try:
   _thread.start_new_thread( test, ("Thread-1", 0, ) )
   _thread.start_new_thread( test, ("Thread-2", 0, ) )
   _thread.start_new_thread( test, ("Thread-3", 0, ) )
   _thread.start_new_thread( test, ("Thread-4", 0, ) )
   _thread.start_new_thread( test, ("Thread-5", 0, ) )
   _thread.start_new_thread( test, ("Thread-6", 0, ) )
   _thread.start_new_thread( test, ("Thread-7", 0, ) )
   _thread.start_new_thread( test, ("Thread-8", 0, ) )
   _thread.start_new_thread( test, ("Thread-9", 0, ) )
   _thread.start_new_thread( test, ("Thread-10", 0, ) )



except:
   print ("Error: 无法启动线程")

while 1:
   pass
