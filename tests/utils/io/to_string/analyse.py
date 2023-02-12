print("input the file name:")
f_name = input()
with open(f_name, "r") as f:
    lines = f.readlines()

cnt = 0
total_time = 0.0
for line in lines:
    if "seconds time elapsed" in line:
        total_time += float(line.split()[0])
        cnt += 1
print(f"average time:{total_time / cnt}")
