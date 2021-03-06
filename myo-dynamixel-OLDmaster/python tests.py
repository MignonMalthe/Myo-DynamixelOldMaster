# The MIT License (MIT)
#
# Copyright (c) 2017 Niklas Rosenstein
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

from matplotlib import pyplot as plt
from collections import deque
from threading import Lock, Thread
import numpy as np
import time
import myo



class EmgCollector(myo.DeviceListener):
  """
  Collects EMG data in a queue with *n* maximum number of elements.
  """

  def __init__(self):
    print("__init__")
    self.lock = Lock()
    self.emg_data_queue = deque(maxlen=50)

  def get_emg_data(self):
    #print("getemgdata")
    with self.lock:
      return list(self.emg_data_queue)

  # myo.DeviceListener

  def on_connected(self, event):
    print("connected")
    event.device.stream_emg(True)

  def on_emg(self, event):
    #print("onemg")
    with self.lock:
      self.emg_data_queue.append((event.timestamp, event.emg))



    

    #emg_data = self.listener.get_emg_data()
    #emg_data = np.array([x[1] for x in emg_data]).T
    #emg_data = map(abs, emg_data)

class readingtest(object):
  def __init__(self, listener):
    self.listener = listener
  def main(self):
    time.sleep(2)
    while True:
        time.sleep(0.1)
        emg_data = self.listener.get_emg_data()
        #print(emg_data)
        emg_data = np.array([x[1] for x in emg_data]).T
        emg_data = np.absolute(emg_data)
        emg_data = np.average(emg_data)
        print(int(emg_data))
        time.sleep(0.00001)


def main():
  print("Main starts")
  myo.init()
  hub = myo.Hub()
  listener = EmgCollector()
  time.sleep(2)
  with hub.run_in_background(listener.on_event):
      readingtest(listener).main()
      
   



if __name__ == '__main__':
  main()
