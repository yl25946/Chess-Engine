def SGD_main():

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("Using device {}".format(device))

    print("Preparing Training Data...")
    train_data = pd.read_csv("archive/chessData.csv")
    train_data = train_data[:MAX_DATA_TRAIN]
    train_data["Evaluation"] = train_data["Evaluation"].map(eval_to_int)
    trainset = ChessDataset(train_data)

    print("Preparing Test Data...")
    test_data = pd.read_csv("archive/tactic_evals.csv")
    test_data = test_data[:MAX_DATA_TEST]
    test_data["Evaluation"] = test_data["Evaluation"].map(eval_to_int)
    testset = ChessDataset(test_data)

    print("Converting to pytorch Dataset...")

    trainloader = DataLoader(
        trainset, batch_size=batch_size, shuffle=True, num_workers=0)

    testloader = DataLoader(testset, batch_size=batch_size,
                            shuffle=False, num_workers=0)

    net = Net().to(device)
    criterion = nn.MSELoss()
    optimizer = optim.SGD(net.parameters(), lr)

    for epoch in range(10):  # loop over the dataset multiple times

        running_loss = 0.0
        for i, data in enumerate(trainloader, 0):
            inputs, labels = data
            inputs = inputs.to(device)
            labels = labels.to(device)
            optimizer.zero_grad()

            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            # print statistics
            running_loss += loss.item()
            if i % 2000 == 1999:    # print every 2000 mini-batches
                # denominator for loss should represent the number of positions evaluated
                # independent of the batch size
                print('[%d, %5d] loss: %.3f' %
                      (epoch + 1, i + 1, running_loss / (2000*len(labels))))
                running_loss = 0.0

    print('Finished Training')

    PATH = './chessEvaluation.pth'
    torch.save(net.state_dict(), PATH)

    print('Evaluating model')

    count = 0
    total_loss = 0
    # since we're not training, we don't need to calculate the gradients for our outputs
    with torch.no_grad():
        for data in testloader:
            inputs, labels = data
            inputs = inputs.to(device)
            labels = labels.to(device)
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            # print("Correct eval: {}, Predicted eval: {}, loss: {}".format(labels, outputs, loss))

            # count should represent the number of positions evaluated
            # independent of the batch size
            count += len(labels)
            total_loss += loss
            if count % AVERAGE_ERROR == 0:
                print('Average error of the model on the {} tactics positions is {}'.format(
                    count, loss/count))
    # print('Average error of the model on the {} tactics positions is {}'.format(count, loss/count))
