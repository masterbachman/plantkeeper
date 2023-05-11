import subprocess

def restart_raspberry_pi():
    command = "sudo shutdown -r now"
    subprocess.call(command, shell=True)

if __name__ == "__main__":
    restart_raspberry_pi()
