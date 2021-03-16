import subprocess
import yaml
from termcolor import colored

tested = 0
passed = 0
failed = 0
print("=========APP TESTS=========")

def run(cmd):
    return subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

with open("./tests/tests_list.yml", "r") as f:
    tests = yaml.load(f)

for test, data in tests.items():
    tested += 1
    ref = run(data["ref"])
    cmd = run(data["cmd"])

    errors = []
    if ref.stdout != cmd.stdout:
        errors.append("-STDOUT FAILED-")
    if ref.stderr != cmd.stderr:
        errors.append("-STDERR FAILED-")
    if ref.returncode != cmd.returncode:
        errors.append("-RETURN CODE FAILED-")

    
    if errors:
        res = "".join(errors)
        failed += 1
    else:
        res = colored("PASSED", 'green')
        passed += 1
    
    print("===" * 30)
    print("{id}: {desc} ==>".format(id=test, desc=data["desc"]), 
        colored("{res}".format(res=res), 'red'))

    if "-STDOUT FAILED-" in errors:
        print("----stdout returned:\n" + colored(cmd.stdout.decode(), 'red'))
        print("----stdout expected:\n" + colored(ref.stdout.decode(), 'blue'))

    if "-STDERR FAILED-" in errors:
        print("----stderr returned:\n" + colored(cmd.stderr.decode(), 'red'))
        print("----stderr expected:\n" + colored(ref.stderr.decode(), 'blue'))

    if "-RETURN CODE FAILED-" in errors:
        print("----return code returned:\n" + colored(str(cmd.returncode), 'red'))
        print("----return code expected:\n" + colored(str(ref.returncode), 'blue'))

print("\nTEST SUMMARY")
print("TESTED: ", tested)
print("FAILED: ", failed)
print("PASSED: ", passed)
print("SUCCEED:", (tested - failed) * 100 / tested,"%")