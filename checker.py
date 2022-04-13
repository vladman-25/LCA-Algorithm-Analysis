from subprocess import call
from pathlib import Path
from os import walk
import hashlib
import argparse
import sys
import logging
from shutil import copyfile
from filecmp import cmp as diff


def run_system_command(cmd: str,
                       shell: bool = False,
                       err_msg: str = None,
                       verbose: bool = True,
                       split: bool = True,
                       stdout=None,
                       stderr=None) -> int:
    """
    :param cmd: A string with the terminal command invoking an external program
    :param shell: Whether the command should be executed through the shell
    :param err_msg: Error message to print if execution fails
    :param verbose: Whether to print the command to the standard output stream
    :param split: Whether to split the tokens in the command string
    :param stdout: file pointer to redirect stdout to
    :param stderr: file pointer to redirect stderr to
    :return: Return code
    """
    if verbose:
        sys.stdout.write("Run cmd: {}\n".format(cmd))
    if split:
        cmd = cmd.split()
    rc = call(cmd, shell=shell, stdout=stdout, stderr=stderr)
    if err_msg and rc:
        sys.stderr.write(err_msg)
        exit(rc)
    return rc


def setup_logging(args):
    msg_format = '%(asctime)s:%(levelname)s: %(message)s'
    formatter = logging.Formatter(msg_format, datefmt='%H:%M:%S')
    handler = logging.StreamHandler()
    handler.setFormatter(formatter)
    logging.getLogger().addHandler(handler)
    logging.getLogger().setLevel(logging.DEBUG)


def check_makefile(args):
    options = ["makefile", "./Makefile"]
    rules = ["build", "run-p1", "run-p2", "run-best"]

    for file in options:
        file = Path(file)
        if file.exists():
            logging.info("Found Makefile!")

            with open(file, "r") as f:
                content = f.read()
                for rule in rules:
                    if rule not in content:
                        logging.warning(
                            "Rule {} not found in Makefile!".format(rule))
                    elif rule == "build":
                        call("make build", shell=True)
            break
    else:
        logging.critical("No Makefile found!")


def check_readme(args):
    options = ["README", "readme", "Readme"]

    for file in options:
        file = Path(file)
        if file.exists():
            logging.info("Found Readme")
            with open(file, "r") as f:
                content = f.read()
                if len(content) < 10:
                    logging.warning(
                        "Readme file is very short - be sure to include all the required information")
            break
    else:
        logging.critical("No README file found!")


def check_tests(args):
    input_path = Path("./in")
    output_path = Path("./out")
    tests = []

    if not input_path.exists() or not input_path.is_dir():
        logging.critical("No folder for the input tests found!")
        return None

    if not output_path.exists() or not output_path.is_dir():
        logging.critical("No folder for the output tests found!")
        return None

    for (_, _, input_tests) in walk(input_path):

        input_tests = [file for file in input_tests if file.endswith(".in")]
        if len(input_tests) < 10:
            logging.critical(
                "Only {} input tests generated!".format(len(input_tests)))
        else:
            logging.info("Found {} tests.".format(len(input_tests)))

        unique = set()
        for test in input_tests:
            contents = open(input_path / test, "r").read().encode("utf-8")
            checksum = hashlib.md5(contents).hexdigest()
            unique.add(checksum)

        if len(input_tests) != len(unique):
            logging.critical(
                "Unique tests: {} / {}!".format(len(unique), len(input_tests)))

    for (_, _, output_tests) in walk(output_path):

        output_tests = [file for file in output_tests if file.endswith(".out")]

        for test in output_tests:
            test_id = test[:-4].replace("test", "")
            test_out = "test{}.out".format(test_id)
            if test_out not in output_tests:
                logging.critical(
                    "Missing output test for test {}!".format(test_id))
            else:
                tests.append((test_id,
                              input_path / "test{}.in".format(test_id),
                              output_path / "test{}.out".format(test_id)))

    return tests


def check_folder_structure(args):
    check_makefile(args)
    check_readme(args)
    return check_tests(args)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--algo', type=str,
                        help="Tells makefile which rule to run (e.g. --algo p1 will trigger rule make run-p1",
                        required=True)
    parser.add_argument('--task', type=int,
                        help="Task ID (e.g. --task 1 for Max Flow",
                        required=True)
    parser.add_argument('--run-test', type=int)
    args = parser.parse_args()
    setup_logging(args)

    tests = check_folder_structure(args)
    for id, input_path, output_path in tests:
        logging.info("Running run-{} test {}..".format(args.algo, id))
        copyfile(input_path, "./test.in")
        call("make run-{}".format(args.algo), shell=True)
        if not diff(output_path, "./test.out"):
            logging.critical("Error trying to run test {}".format(id))
    logging.info("Done!")


if __name__ == "__main__":
    main()