process.removeAllListeners('warning'); // Suppress all warnings
/**
 * Bank Management System
 * ----------------------
 * This script manages user accounts, transactions, and admin operations using Appwrite SDK.
 * 
 * Icons Used:
 * 🔐 - Admin Authentication
 * 🧾 - Fetching Accounts
 * 📛🔑 - Changing User Data [Username & Password]
 * 💰 - Balance Update
 */

// Importing necessary modules from Appwrite SDK
const { Client, Databases, ID, Query } = require("appwrite");
const argon2 = require("argon2");
const nodemailer = require("nodemailer");

// All Configs Keys
const config = {
  PROJECT_ID: "674c43ae00145d935579",
  DATABASE_ID: "674c4494001d200ea966",
  COLLECTION_ID_USERS: "674c44b80009591a6769",
  COLLECTION_ID_PENDING_ACCOUNTS: "674c4819002cdd664013",
  COLLECTION_ID_TRANSACTIONS: "674c4956002350911179",
  COLLECTION_ID_ADMIN: "674c4ae4002be0c8fa56",
  GMAIL_USER: "autfinance.bank@gmail.com",
  GMAIL_PASS: "vwim wcwf chif odta"
};

// Initialize Appwrite client and databases
const client = new Client()
  .setEndpoint("https://cloud.appwrite.io/v1")
  .setProject(config.PROJECT_ID);
const databases = new Databases(client);

// Extract command line arguments
const args = process.argv.slice(2);
const command = args[0];

/*<=============================================================================================================>
                        !! FUNCTIONS RELATED TO FETCHING ACCOUNTS 🧾 !!                                             
  <=============================================================================================================>
*/

// Fetch accounts based on command line input
if (command === "FetchAccounts") {
  FetchAccounts();
} else if (command === "FetchPendingAccounts") {
  FetchPendingAccounts();
}

/**
 * Fetches user account details from the database and logs them.
 * @returns {void}
 */
async function FetchAccounts() {
  try {
    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_USERS,
      []
    );
    for (let i = 0; i < response.total; i++) {
      const user = response.documents[i];
      const { username, email, accountType, balance, CNIC, Key } = user;

      console.log(
        `${username} ${email} ${accountType} ${balance} ${CNIC} ${Key}`
      );
    }
  } catch (error) {
    console.error("Error:", error);
  }
}
/**
 * Fetches pending account details from the database and logs specific user information.
 * @returns {void}
 */
async function FetchPendingAccounts() {
  try {
    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_PENDING_ACCOUNTS,
      []
    );
    for (let i = 0; i < response.total; i++) {
      const user = response.documents[i];
      const { username, email, accountType, CNIC } = user;

      console.log(`${username} ${email} ${accountType}  ${CNIC}`);
    }
  } catch (error) {
    console.error("Error:", error);
  }
}

/*<=============================================================================================================>*/

/*<=============================================================================================================>
                        !! MOVING TO ACCOUNTS AFTER ADMIN APPROVAL 🧾 !!                                             
  <=============================================================================================================>
*/

// Move pending account to active status
if (command === "MoveToAccounts") {
  const cnic = args[1];
  MoveToAccounts(cnic);
}

/**
 * Moves a pending account to active status after admin approval.
 * @param {string} username - Username of the account.
 * @param {string} pass - Password of the account.
 * @param {string} email - Email of the account.
 * @param {string} accType - Account type ('Savings' or 'Current').
 * @param {string} cnic - CNIC of the account holder.
 * @returns {void}
 */
async function MoveToAccounts(cnic) {
  try {
    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_PENDING_ACCOUNTS,
      [Query.equal("CNIC", cnic)]
    );

    const { username, email, accountType, CNIC, password, Key } = response.documents[0];
    let payload = {
      username: username,
      password: password,
      email: email,
      accountType: accountType,
      balance: 0,
      CNIC: CNIC,
      Key: Key
    };
    // Create a new user
    const promise = await databases.createDocument(
      config.DATABASE_ID,
      config.COLLECTION_ID_USERS,
      ID.unique(),
      payload
    );
    DeleteAccount(cnic, config.COLLECTION_ID_PENDING_ACCOUNTS);
    console.log("SUCCESS");
  } catch (error) {
    console.error("Error:", error);
  }
}
/*<=============================================================================================================>*/

/*<=============================================================================================================>
                        !! ADMIN AUTHENTICATION 🔐 !!                                             
  <=============================================================================================================>
*/

// Admin login authentication
if (command === "AdminLogin") {
  const username = args[1];
  const password = args[2];
  AdminLogin(username, password);
}

/**
 * Authenticates admin login credentials.
 * @param {string} username - Admin username.
 * @param {string} password - Admin password.
 * @returns {void}
 */
async function AdminLogin(username, password) {
  try {

    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_ADMIN,
      []
    );
    if (response.documents[0].username === username) {
      const isPasswordValid = await argon2.verify(response.documents[0].password, password);
      if (isPasswordValid) {
        console.log("SUCCESS");
      }
    }
  } catch (error) {
    console.log("Error Occurred", error);
  }
};
/*<=============================================================================================================>*/

/*<=============================================================================================================>
                        !! USER REGISTRATION 🔐 !!                                             
  <=============================================================================================================>
*/

// Register a new user account
if (command === "Register") {
  const username = args[1];
  const password = args[2];
  const email = args[3];
  const accountType = args[4];
  const cnic = args[5];
  const key = args[6];
  Register(username, password, email, accountType, cnic, key);
}

/**
 * Registers a new user account.
 * @param {string} username - Username for the new account.
 * @param {string} pass - Password for the new account.
 * @param {string} email - Email address of the new account.
 * @param {string} accType - Account type ('S' for Savings or 'C' for Current).
 * @param {string} cnic - CNIC of the new account holder.
 * @param {string} key - key for forgot password of the new account .
 * @returns {void}
 */
async function Register(username, pass, email, accType, cnic, key) {
  try {
    const hashedPassword = await argon2.hash(pass);
    let payload = {
      username: username,
      password: hashedPassword,
      email: email,
      accountType: accType.toUpperCase() === "S" ? "Savings" : "Current",
      balance: 0,
      CNIC: cnic,
      Key: key
    };
    // Create a new user
    const promise = await databases.createDocument(
      config.DATABASE_ID,
      config.COLLECTION_ID_PENDING_ACCOUNTS,
      ID.unique(),
      payload
    );
    console.log("USER CREATED SUCCESSFULLY");
  } catch (error) {
    console.error("Error:", error);
  }
}

/*<=============================================================================================================>*/

/*<=============================================================================================================>
                        !! USER LOGIN 🔐 !!                                             
  <=============================================================================================================>
*/
if (command === "UserLogin") {
  const cnic = args[1];
  const password = args[2];
  UserLogin(cnic, password);
}
async function UserLogin(cnic, password) {
  try {
    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_USERS,
      [Query.equal("CNIC", cnic)]
    );
    if (response.documents[0].CNIC === cnic) {
      const isPasswordValid = await argon2.verify(response.documents[0].password, password);
      if (isPasswordValid) {
        console.log("SUCCESS");
      } else {
        console.log("Incorrect password");
      }
    }
  } catch (error) {
    console.log("Error Occurred", error);
  }
}
/*<=============================================================================================================>*/

/*<=============================================================================================================>
                        !! BALANCE UPDATE 💰 !!                                             
  <=============================================================================================================>
*/

// Update user account balance
if (command === "BalanceUpdate") {
  const cnic = args[1];
  const newBalance = args[2];
  UpdateUserBalance(cnic, newBalance);
}

/**
 * Updates the balance of a user account.
 * @param {string} cnic - CNIC of the user.
 * @param {string} newBalance - New balance to be updated.
 * @returns {void}
 */
async function UpdateUserBalance(cnic, newBalance) {
  let response = await databases.listDocuments(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    [Query.equal("CNIC", cnic)]
  );
  let documentID = response.documents[0].$id;

  let promise = await databases.updateDocument(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    documentID,
    { balance: parseFloat(newBalance) }
  );
}

/*<=============================================================================================================>*/

/*<=============================================================================================================>
                !! CHANGING USER DATA [USERNAME & PASSWORD] 📛🔑 !!                                             
  <=============================================================================================================>
*/

// Change user account password
if (command === "ChangeUserPassword") {
  const cnic = args[1];
  const newPasword = args[2];
  ChangeUserPassword(cnic, newPasword);
}

/**
 * Changes the password of a user account.
 * @param {string} cnic - CNIC of the user.
 * @param {string} newPasword - New password to set for the account.
 * @returns {void}
 */
async function ChangeUserPassword(cnic, newPasword) {
  const hashedPassword = await argon2.hash(newPasword);
  let response = await databases.listDocuments(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    [Query.equal("CNIC", cnic)]
  );
  let documentID = response.documents[0].$id;

  let promise = await databases.updateDocument(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    documentID,
    { password: hashedPassword }
  );
}

// Change user account username
if (command === "ChangeUserUsername") {
  const cnic = args[1];
  const newUsername = args[2];
  ChangeUserUsername(cnic, newUsername);
}

/**
 * Changes the username of a user account.
 * @param {string} cnic - CNIC of the user.
 * @param {string} newUsername - New username to set for the account.
 * @returns {void}
 */
async function ChangeUserUsername(cnic, newUsername) {
  let response = await databases.listDocuments(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    [Query.equal("CNIC", cnic)]
  );
  let documentID = response.documents[0].$id;

  let promise = await databases.updateDocument(
    config.DATABASE_ID,
    config.COLLECTION_ID_USERS,
    documentID,
    { username: newUsername }
  );
  console.log("USERNAME_UPDATED");
}

/*<=============================================================================================================>*/

/*<=============================================================================================================>
                !! LOG TRANSACTION IN THE DATABASE  !!                                             
  <=============================================================================================================>
*/

// Log transaction in the database
if (command === "LogTransaction") {
  const cnic = args[1];
  const timestamp = args[2];
  const type = args[3];
  const amount = parseFloat(args[4]);
  if (args[5]) {
    const ToORFromCNIC = args[5];
    logTransaction(cnic, timestamp, type, amount, ToORFromCNIC);
  } else {
    logTransaction(cnic, timestamp, type, amount);
  }
}

/**
 * Logs a transaction into the transactions database.
 * @param {string} cnic - CNIC of the user.
 * @param {string} timestamp - Timestamp of the transaction.
 * @param {string} type - Type of transaction ('Received', 'Sent', 'Transfer').
 * @param {number} amount - Amount of money involved in the transaction.
 * @param {string} [ToORFromCNIC] - CNIC of the recipient or sender in case of 'Transfer'.
 * @returns {void}
 */
async function logTransaction(cnic, timestamp, type, amount, ToORFromCNIC) {
  try {
    const payload = {
      cnic: cnic,
      timestamp: timestamp,
      type: type,
      amount: amount,
      ToORFromCNIC: ToORFromCNIC,
    };
    await databases.createDocument(
      config.DATABASE_ID,
      config.COLLECTION_ID_TRANSACTIONS,
      ID.unique(),
      payload
    );
    console.log("TRANSACTION_LOGGED");
  } catch (error) {
    console.error(error);
  }
}

// Fetch user transactions from the database
if (command === "FetchTransactions") {
  const cnic = args[1];
  fetchTransactions(cnic);
}

/**
 * Fetches user transactions from the transactions database.
 * @param {string} cnic - CNIC of the user.
 * @returns {void}
 */
async function fetchTransactions(cnic) {
  try {
    const response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_TRANSACTIONS,
      [Query.equal("cnic", cnic)]
    );
    let transactions = response.documents.map((doc) => ({
      CNIC: doc.cnic,
      Timestamp: doc.timestamp,
      Type: doc.type,
      Amount: `$${doc.amount}`,
      "From(CNIC)": doc.type === "Received" && doc.ToORFromCNIC ? doc.ToORFromCNIC : "",
      "To(CNIC)": doc.type === "Transfer" && doc.ToORFromCNIC ? doc.ToORFromCNIC : ""
    }));
    // Check if any "From(CNIC)" or "To(CNIC)" fields have values
    const includeFrom = transactions.some(transaction => transaction["From(CNIC)"]);
    const includeTo = transactions.some(transaction => transaction["To(CNIC)"]);

    // Filter out empty fields
    transactions = transactions.map(transaction => {
      const filteredTransaction = { ...transaction };
      if (!includeFrom) delete filteredTransaction["From(CNIC)"];
      if (!includeTo) delete filteredTransaction["To(CNIC)"];
      return filteredTransaction;
    });
    // Calculate column widths
    const columns = ["CNIC", "Timestamp", "Type", "Amount"];
    if (includeFrom) columns.push("From(CNIC)");
    if (includeTo) columns.push("To(CNIC)");

    const colWidths = columns.map(col => col.length);
    transactions.forEach(transaction => {
      columns.forEach((col, i) => {
        colWidths[i] = Math.max(colWidths[i], (transaction[col] || "").toString().length);
      });
    });

    // Create the table header
    const separator = colWidths.map(width => '-'.repeat(width + 2.1)).join('+');
    const header = columns.map((col, i) => col.padEnd(colWidths[i])).join(' | ');

    // Print the table
    console.log(` ${separator}`);
    console.log(` | ${header} |`);
    console.log(` ${separator}`);
    transactions.forEach(transaction => {
      const row = columns.map((col, i) => (transaction[col] || "").toString().padEnd(colWidths[i])).join(' | ');
      console.log(` | ${row} |`);
    });
    console.log(` ${separator}`);
  } catch (error) {
    console.error(error);
  }
}

// Delete user account from the database
if (command === "DeleteAccount") {
  const cnic = args[1];
  DeleteAccount(cnic, config.COLLECTION_ID_USERS);
}

/**
 * Deletes a user account from the database.
 * @param {string} cnic - CNIC of the user.
 * @param {string} CollectionID - ID of the collection from which the account will be deleted.
 * @returns {void}
 */
async function DeleteAccount(cnic, CollectionID) {
  try {
    let response = await databases.listDocuments(config.DATABASE_ID, CollectionID, [
      Query.equal("CNIC", cnic),
    ]);
    let documentID = response.documents[0].$id;

    let promise = await databases.deleteDocument(
      config.DATABASE_ID,
      CollectionID,
      documentID
    );

    let res = await databases.listDocuments(config.DATABASE_ID, config.COLLECTION_ID_TRANSACTIONS, [
      Query.equal("cnic", cnic),
    ]);

    res.documents.forEach(async (doc) => {
      let promise = await databases.deleteDocument(
        config.DATABASE_ID,
        config.COLLECTION_ID_TRANSACTIONS,
        doc.$id
      );
    })
    console.log("ACCOUNT_DELETED");
  } catch (error) {
    console.error("Error:", error);
  }
}

if (command === "forgot_password") {
  const cnic = args[1];
  const key = args[2];

  forgot_password(cnic, key);
}
async function forgot_password(cnic, key) {
  try {
    let response = await databases.listDocuments(
      config.DATABASE_ID,
      config.COLLECTION_ID_USERS,
      [Query.equal("CNIC", cnic), Query.equal("Key", key)]
    );
    if (response.total > 0) {
      console.log("SUCCESS");
    }

  } catch (e) {
    console.log("ERROR ");
  }
}

if (command === "sendOTPEmail") {
  const email = args[1];
  const otp = args[2];
  const reason = args.slice(3).join(' ');
  sendOTPEmail(email, otp, reason);
}

async function sendOTPEmail(email, otp, reason) {
  try {
    let date = new Date();
    let options = { day: 'numeric', month: 'short', year: 'numeric' };
    let formattedDate = date.toLocaleDateString('en-GB', options);
    // Split the formatted date to insert a comma
    let [day, month, year] = formattedDate.split(' ');
    formattedDate = `${day} ${month} ${year}`;

    let transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: config.GMAIL_USER,
        pass: config.GMAIL_PASS
      }
    });

    let mailOptions = {
      from: config.GMAIL_USER,
      to: email,
      subject: 'Your OTP Code',
      html: `<!DOCTYPE html>
      <html lang="en">
        <head>
          <meta charset="UTF-8" />
          <meta name="viewport" content="width=device-width, initial-scale=1.0" />
          <meta http-equiv="X-UA-Compatible" content="ie=edge" />
          <title>OTP</title>
      
          <link
            href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600&display=swap"
            rel="stylesheet"
          />
        </head>
        <body
          style="
            margin: 0;
            font-family: 'Poppins', sans-serif;
            background: #ffffff;
            font-size: 14px;
          "
        >
          <div
            style="
              max-width: 680px;
              margin: 0 auto;
              padding: 45px 30px 60px;
              background: #f4f7ff;
              background-image: url(https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661497957196_595865/email-template-background-banner);
              background-repeat: no-repeat;
              background-size: 800px 452px;
              background-position: top center;
              font-size: 14px;
              color: #434343;
            "
          >
            <header>
              <table style="width: 100%;">
                <tbody>
                  <tr style="height: 0;">
                    <td>
                      <img
                        alt="AUT Bank Logo"
                        src="https://raw.githubusercontent.com/ahad324/BankManagementSystemCpp/main/Logo.jpeg"
                        height="100px"
                      />
                    </td>
                    <td style="text-align: right;">
                      <span
                        style="font-size: 16px; line-height: 30px; color: #ffffff;"
                        >${formattedDate}</span
                      >
                    </td>
                  </tr>
                </tbody>
              </table>
            </header>
      
            <main>
              <div
                style="
                  margin: 0;
                  margin-top: 70px;
                  padding: 92px 30px 115px;
                  background: #ffffff;
                  border-radius: 30px;
                  text-align: center;
                  box-shadow:0 0 20px rgba(0,0,0,0.5);
                "
              >
                <div style="width: 100%; max-width: 489px; margin: 0 auto;">
                  <h1
                    style="
                      margin: 0;
                      font-size: 24px;
                      font-weight: 500;
                      color: #1f1f1f;
                    "
                  >
                    Your OTP
                  </h1>
                  <p
                    style="
                      margin: 0;
                      margin-top: 17px;
                      font-size: 16px;
                      font-weight: 500;
                    "
                  >
                    Hey Welcome !,
                  </p>
                  <p
                    style="
                      margin: 0;
                      margin-top: 17px;
                      font-weight: 500;
                      letter-spacing: 0.56px;
                    "
                  >
                    Thank you for choosing AUT Finance Bank. Use the following OTP
                    to complete the procedure of ${reason}. OTP is
                    valid for
                    <span style="font-weight: 600; color: #1f1f1f;">5 minutes</span>.
                    Do not share this code with others.
                  </p>
                  <p
                    style="
                      margin: 0;
                      margin-top: 60px;
                      font-size: 40px;
                      font-weight: 600;
                      letter-spacing: 25px;
                      color: #ba3d4f;
                    "
                  >
                    ${otp}
                  </p>
                </div>
              </div>
      
              <p
                style="
                  max-width: 400px;
                  margin: 0 auto;
                  margin-top: 90px;
                  text-align: center;
                  font-weight: 500;
                  color: #8c8c8c;
                "
              >
                Need help? Ask at
                <a
                  href="mailto:autfinance.bank@gmail.com"
                  style="color: #499fb6; text-decoration: none;"
                  >autfinance.bank@gmail.com</a
                >
                or visit our
                <a
                  href="https://bmscpp.netlify.app/"
                  target="_blank"
                  style="color: #499fb6; text-decoration: none;"
                  >Help Center</a
                >
              </p>
            </main>
      
            <footer
              style="
                width: 100%;
                max-width: 490px;
                margin: 20px auto 0;
                text-align: center;
                border-top: 1px solid #e6ebf1;
              "
            >
              <p
                style="
                  margin: 0;
                  margin-top: 40px;
                  font-size: 16px;
                  font-weight: 600;
                  color: #434343;
                "
              >
                AUT Finance Bank
              </p>
              <p style="margin: 0; margin-top: 8px; color: #434343;">
                Address 540, Lahore, Punjab.
              </p>
              <div style="margin: 0; margin-top: 16px;">
                <a href="" target="_blank" style="display: inline-block;">
                  <img
                    width="36px"
                    alt="Facebook"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661502815169_682499/email-template-icon-facebook"
                  />
                </a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Instagram"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661504218208_684135/email-template-icon-instagram"
                /></a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Twitter"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661503043040_372004/email-template-icon-twitter"
                  />
                </a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Youtube"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661503195931_210869/email-template-icon-youtube"
                /></a>
              </div>
              <p style="margin: 0; margin-top: 16px; color: #434343;">
                Copyright © 2024 Company. All rights reserved.
              </p>
            </footer>
          </div>
        </body>
      </html>
      
          `,
    };

    await transporter.sendMail(mailOptions);
    console.log('OTP sent successfully');
  } catch (error) {
    console.error('Error sending OTP:', error);
  }
}

if (command === "sendEmail") {
  const email = args[1];
  const message = args.slice(2).join(' ');
  sendEmail(email, message);
}
async function sendEmail(email, message) {
  try {
    let date = new Date();
    let options = { day: 'numeric', month: 'short', year: 'numeric' };
    let formattedDate = date.toLocaleDateString('en-GB', options);
    // Split the formatted date to insert a comma
    let [day, month, year] = formattedDate.split(' ');
    formattedDate = `${day} ${month} ${year}`;

    let transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: config.GMAIL_USER,
        pass: config.GMAIL_PASS
      }
    });

    let mailOptions = {
      from: config.GMAIL_USER,
      to: email,
      subject: 'Message from AUT Finance Bank',
      html: `<!DOCTYPE html>
      <html lang="en">
        <head>
          <meta charset="UTF-8" />
          <meta name="viewport" content="width=device-width, initial-scale=1.0" />
          <meta http-equiv="X-UA-Compatible" content="ie=edge" />
          <title>AUT Finance Bank</title>
      
          <link
            href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600&display=swap"
            rel="stylesheet"
          />
        </head>
        <body
          style="
            margin: 0;
            font-family: 'Poppins', sans-serif;
            background: #ffffff;
            font-size: 14px;
          "
        >
          <div
            style="
              max-width: 680px;
              margin: 0 auto;
              padding: 45px 30px 60px;
              background: #f4f7ff;
              background-image: url(https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661497957196_595865/email-template-background-banner);
              background-repeat: no-repeat;
              background-size: 800px 452px;
              background-position: top center;
              font-size: 14px;
              color: #434343;
            "
          >
            <header>
              <table style="width: 100%;">
                <tbody>
                  <tr style="height: 0;">
                    <td>
                      <img
                        alt="AUT Bank Logo"
                        src="https://raw.githubusercontent.com/ahad324/BankManagementSystemCpp/main/Logo.jpeg"
                        height="100px"
                      />
                    </td>
                    <td style="text-align: right;">
                      <span
                        style="font-size: 16px; line-height: 30px; color: #ffffff;"
                        >${formattedDate}</span
                      >
                    </td>
                  </tr>
                </tbody>
              </table>
            </header>
      
            <main>
              <div
                style="
                  margin: 0;
                  margin-top: 70px;
                  padding: 92px 30px 115px;
                  background: #ffffff;
                  border-radius: 30px;
                  text-align: center;
                  box-shadow:0 0 20px rgba(0,0,0,0.5);
                "
              >
                <div style="width: 100%; max-width: 489px; margin: 0 auto;">
                  <h1
                    style="
                      margin: 0;
                      font-size: 24px;
                      font-weight: 500;
                      color: #1f1f1f;
                    "
                  >
                    Message from Admin
                  </h1>
                  <p
                    style="
                      margin: 0;
                      margin-top: 17px;
                      font-size: 16px;
                      font-weight: 500;
                    "
                  >
                    Dear Customer! ,
                  </p>
                  <p
                    style="
                      margin: 0;
                      margin-top: 17px;
                      font-weight: 500;
                      letter-spacing: 0.56px;
                    "
                  >
                    ${message}
                  </p>
                </div>
              </div>
      
              <p
                style="
                  max-width: 400px;
                  margin: 0 auto;
                  margin-top: 90px;
                  text-align: center;
                  font-weight: 500;
                  color: #8c8c8c;
                "
              >
                Need help? Ask at
                <a
                  href="mailto:autfinance.bank@gmail.com"
                  style="color: #499fb6; text-decoration: none;"
                  >autfinance.bank@gmail.com</a
                >
                or visit our
                <a
                  href="https://bmscpp.netlify.app/"
                  target="_blank"
                  style="color: #499fb6; text-decoration: none;"
                  >Help Center</a
                >
              </p>
            </main>
      
            <footer
              style="
                width: 100%;
                max-width: 490px;
                margin: 20px auto 0;
                text-align: center;
                border-top: 1px solid #e6ebf1;
              "
            >
              <p
                style="
                  margin: 0;
                  margin-top: 40px;
                  font-size: 16px;
                  font-weight: 600;
                  color: #434343;
                "
              >
                AUT Finance Bank
              </p>
              <p style="margin: 0; margin-top: 8px; color: #434343;">
                Address 540, Lahore, Punjab.
              </p>
              <div style="margin: 0; margin-top: 16px;">
                <a href="" target="_blank" style="display: inline-block;">
                  <img
                    width="36px"
                    alt="Facebook"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661502815169_682499/email-template-icon-facebook"
                  />
                </a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Instagram"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661504218208_684135/email-template-icon-instagram"
                /></a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Twitter"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661503043040_372004/email-template-icon-twitter"
                  />
                </a>
                <a
                  href=""
                  target="_blank"
                  style="display: inline-block; margin-left: 8px;"
                >
                  <img
                    width="36px"
                    alt="Youtube"
                    src="https://archisketch-resources.s3.ap-northeast-2.amazonaws.com/vrstyler/1661503195931_210869/email-template-icon-youtube"
                /></a>
              </div>
              <p style="margin: 0; margin-top: 16px; color: #434343;">
                Copyright © 2024 Company. All rights reserved.
              </p>
            </footer>
          </div>
        </body>
      </html>
      
          `,
    };

    await transporter.sendMail(mailOptions);
    console.log('SUCCESS');
  } catch (error) {
    console.error('Error sending OTP:', error);
  }
}
