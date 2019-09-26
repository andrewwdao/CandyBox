import webEmpath
import AudioRecord

if __name__ == "__main__":
    while True:
        AudioRecord.start()
        if webEmpath.check(AudioRecord.filename):
            AudioRecord.save_joy(webEmpath.joy_now())